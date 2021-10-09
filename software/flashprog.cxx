// SPDX-License-Identifier: BSD-3-Clause
#include <vector>
#include <thread>
#include <chrono>
#include <substrate/utility>
#include <substrate/units>
#include <substrate/console>
#include <substrate/fd>
#include <version.hxx>
#include "args.hxx"
#include "help.hxx"
#include "usbContext.hxx"
#include "usbProtocol.hxx"
#include "progress.hxx"

// TODO: Add ChaiScript support for the flashing algorithms.

namespace flashprog
{
	constexpr static auto options{substrate::make_array<args::option_t>(
	{
		{"--version"sv, argType_t::version},
		{"--help"sv, argType_t::help},
		{"-h"sv, argType_t::help},
		{"listDevices"sv, argType_t::listDevices},
		{"list"sv, argType_t::list},
		{"erase"sv, argType_t::erase},
		{"read"sv, argType_t::read},
		{"write"sv, argType_t::write},
		{"verifiedWrite"sv, argType_t::verifiedWrite},
	})};

	inline int32_t printHelp() noexcept
	{
		console.info(helpString);
		return 0;
	}
} // namespace flashprog

using namespace flashProto;
using flashprog::args::ensure_t;
using namespace std::literals::chrono_literals;
using namespace substrate;
using flashprog::args::argsTree_t;

constexpr static auto transferBlockSize{4_KiB};

auto requestCount(const usbDeviceHandle_t &device)
{
	responses::deviceCount_t deviceCount{};
	if (!requests::deviceCount_t{}.read(device, 0, deviceCount))
		throw requests::usbError_t{};
	return std::make_tuple(deviceCount.internalCount, deviceCount.externalCount);
}

bool listDevice(const usbDeviceHandle_t &device, const deviceType_t deviceType, const uint8_t deviceNumber) noexcept
{
	responses::listDevice_t listing{};
	if (!requests::listDevice_t{deviceNumber, deviceType}.read(device, 0, listing))
		return false;

	console.info('\t', deviceNumber, ": Manufacturer - "sv, listing.manufacturer,
		", Capacity - "sv, listing.deviceSize, ", Page size - "sv, uint32_t{listing.pageSize},
		", Erase page size - "sv, uint32_t{listing.eraseSize});
	return true;
}

int32_t listDevices(const usbDevice_t &rawDevice)
{
	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	try
	{
		const auto &[internalDeviceCount, externalDeviceCount] = requestCount(device);

		console.info("Programmer has "sv, internalDeviceCount, " internal Flash chips, and "sv,
			externalDeviceCount, " external Flash chips"sv);
		console.info("Internal devices:"sv);
		for (uint8_t interalDevice{0}; interalDevice < internalDeviceCount; ++interalDevice)
			listDevice(device, deviceType_t::internal, interalDevice);
		console.info("External devices:"sv);
		for (uint8_t externalDevice{0}; externalDevice < externalDeviceCount; ++externalDevice)
			listDevice(device, deviceType_t::external, externalDevice);
	}
	catch (const requests::usbError_t &error)
	{
		console.error("Failure while listing devices: "sv, error.what());
		[[maybe_unused]] const auto _{device.releaseInterface(0)};
		return 1;
	}

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

bool targetDevice(const usbDeviceHandle_t &device, const deviceType_t deviceType, const uint8_t deviceNumber) noexcept
	{ return requests::targetDevice_t{deviceNumber, deviceType}.write(device, 0); }

int32_t eraseDevice(const usbDevice_t &rawDevice, const argsTree_t *const eraseArgs)
{
	const auto *const chip{dynamic_cast<flashprog::args::argChip_t *>(eraseArgs->find(argType_t::chip))};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	if (!targetDevice(device, deviceType_t::internal, chip ? chip->chipNumber() : 0U))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	progressBar_t bar{"Erasing chip"sv};
	bar.display();
	const auto startTime{std::chrono::steady_clock::now()};

	if (!requests::erase_t{}.write(device, 0, eraseOperation_t::all))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}
	++bar;

	responses::status_t status{};
	while (!status.eraseComplete)
	{
		std::this_thread::sleep_for(250ms);
		if (!requests::status_t{}.read(device, 0, status))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		++bar;
	}
	bar.close();
	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	if (!device.releaseInterface(0))
		return 1;
	// As complete is non-0 at this point, if it has the value 1, it was a good completion
	// otherwise, the programmer said we asked it to do something insane, so error out.
	return status.eraseComplete == 1 ? 0 : 1;
}

int32_t readDevice(const usbDevice_t &rawDevice, const argsTree_t *const readArgs)
{
	const auto *const chip{dynamic_cast<flashprog::args::argChip_t *>(readArgs->find(argType_t::chip))};
	const auto *const file{dynamic_cast<flashprog::args::argFile_t *>(readArgs->find(argType_t::file))};
	const auto chipNumber{static_cast<uint8_t>(chip ? chip->chipNumber() : 0U)};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	substrate::fd_t fd{file->fileName().data(), O_CREAT | O_WRONLY | O_NOCTTY, substrate::normalMode};
	if (!fd.valid())
	{
		console.error("Failed to open output file '"sv, file->fileName(), "'"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	responses::listDevice_t chipInfo{};
	try
	{
		requests::listDevice_t request{chipNumber, deviceType_t::internal};
		if (!request.read(device, 0, chipInfo))
			throw responses::usbError_t{};
	}
	catch (const responses::usbError_t &error)
	{
		console.error("Error getting target device information: "sv, error.what());
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!targetDevice(device, deviceType_t::internal, chipNumber))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	const auto startTime{std::chrono::steady_clock::now()};

	if (chipInfo.deviceSize > transferBlockSize)
	{
		if (chipInfo.deviceSize % transferBlockSize)
		{
			console.error("Funky device size, is "sv, chipInfo.deviceSize,
				", was expecting a device size that divided by "sv, transferBlockSize);
			if (!device.releaseInterface(0))
				return 1;
			return 1;
		}
		const auto pagesPerBlock{static_cast<uint32_t>(transferBlockSize / chipInfo.pageSize)};
		const auto blockCount{static_cast<uint32_t>(chipInfo.deviceSize / transferBlockSize)};
		progressBar_t bar{"Reading chip "sv, blockCount};
		bar.display();
		for (uint32_t block{}; block < blockCount; ++block)
		{
			const auto page{block * pagesPerBlock};
			if (!requests::read_t{page}.write(device, 0, transferBlockSize))
			{
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}

			std::array<std::byte, transferBlockSize> data{};
			if (!device.readBulk(1, data.data(), data.size()) ||
				!fd.write(data))
			{
				console.error("Failed to read pages "sv, page, ":"sv, page + pagesPerBlock - 1,
					" back from the device"sv);
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}
			++bar;
		}
		bar.close();
	}
	else
	{
		const uint32_t pageSize{chipInfo.pageSize};
		const uint32_t pageCount{chipInfo.deviceSize / pageSize};
		progressBar_t bar{"Reading chip "sv, pageCount};
		// NOLINTNEXTLINE: cppcoreguidelines-avoid-c-arrays
		auto data{std::make_unique<std::byte []>(pageSize)};
		bar.display();
		for (uint32_t page{}; page < pageCount; ++page)
		{
			if (!requests::read_t{page}.write(device, 0))
			{
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}

			if (!device.readBulk(1, data.get(), pageSize) ||
				!fd.write(data, pageSize))
			{
				console.error("Failed to read page "sv, page, " back from the device"sv);
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}
			++bar;
		}
		bar.close();
	}
	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

int32_t writeDevice(const usbDevice_t &rawDevice, const argsTree_t *const writeArgs)
{
	const auto *const chip{dynamic_cast<flashprog::args::argChip_t *>(writeArgs->find(argType_t::chip))};
	const auto *const file{dynamic_cast<flashprog::args::argFile_t *>(writeArgs->find(argType_t::file))};
	const auto chipNumber{static_cast<uint8_t>(chip ? chip->chipNumber() : 0U)};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	substrate::fd_t fd{file->fileName().data(), O_RDONLY | O_NOCTTY};
	if (!fd.valid())
	{
		console.error("Failed to open input file '"sv, file->fileName(), "'"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	responses::listDevice_t chipInfo{};
	try
	{
		requests::listDevice_t request{chipNumber, deviceType_t::internal};
		if (!request.read(device, 0, chipInfo))
			throw responses::usbError_t{};
	}
	catch (const responses::usbError_t &error)
	{
		console.error("Error getting target device information: "sv, error.what());
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	const auto fileLength{fd.length()};
	if (fileLength < 0 || fileLength > chipInfo.deviceSize)
	{
		console.error("The file given is larger than the target device"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!targetDevice(device, deviceType_t::internal, chipNumber))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	const auto startTime{std::chrono::steady_clock::now()};

	if (chipInfo.deviceSize >= transferBlockSize)
	{
		if (chipInfo.deviceSize % transferBlockSize)
		{
			console.error("Funky device size, is "sv, chipInfo.deviceSize,
				", was expecting a device size that divided by "sv, transferBlockSize);
			if (!device.releaseInterface(0))
				return 1;
			return 1;
		}
		const auto pagesPerBlock{static_cast<uint32_t>(transferBlockSize / chipInfo.pageSize)};
		const auto blockCount
		{
			[fileLength] () -> uint32_t
			{
				const auto blocks{fileLength / transferBlockSize};
				const auto remainder{fileLength % transferBlockSize};
				return blocks + (remainder ? 1U : 0U);
			}()
		};
		progressBar_t bar{"Writing chip "sv, blockCount};
		bar.display();
		for (uint32_t block{}; block < blockCount; ++block)
		{
			const auto page{block * pagesPerBlock};
			const auto byteCount{std::min(uint32_t(fileLength) - (block * transferBlockSize), transferBlockSize)};
			if (!requests::write_t{page}.write(device, 0, byteCount))
			{
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}

			std::array<std::byte, transferBlockSize> data{};
			if (!fd.read(data.data(), byteCount) ||
				!device.writeBulk(1, data.data(), byteCount))
			{
				console.error("Failed to write pages "sv, page, ":"sv, page + pagesPerBlock - 1,
					" to the device"sv);
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}
			++bar;
		}
		bar.close();
	}
	else
	{
		const uint32_t pageSize{chipInfo.pageSize};
		const uint32_t pageCount
		{
			[fileLength, pageSize] () -> uint32_t
			{
				const auto pages{fileLength / pageSize};
				const auto remainder{fileLength % pageSize};
				return pages + (remainder ? 1U : 0U);
			}()
		};
		progressBar_t bar{"Writing chip "sv, pageCount};
		// NOLINTNEXTLINE: cppcoreguidelines-avoid-c-arrays
		auto data{std::make_unique<std::byte []>(pageSize)};
		bar.display();
		for (uint32_t page{}; page < pageCount; ++page)
		{
			const auto byteCount{std::min(uint32_t(fileLength) - (page * pageSize), pageSize)};
			if (!requests::write_t{page}.write(device, 0, byteCount))
			{
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}

			if (!fd.read(data, byteCount) ||
				!device.writeBulk(1, data.get(), byteCount))
			{
				console.error("Failed to write page "sv, page, " to the device"sv);
				if (!device.releaseInterface(0))
					return 2;
				return 1;
			}
			++bar;
		}
		bar.close();
	}

	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

/*!
 * flashprog usage:
 *
 * <no command> - List the available SPIFlashProgrammer v2's
 * listDevices - List the available SPIFlashProgrammer v2's
 * --device N - Selects which SPIFlashProgrammer to use
 * list - List the available flash on a given device
 * erase N - Erases the contents of the given device
 * read N file - Reads the contents of the given device into the given file
 * write N file - Writes the contents of the given file into the selected device
 * verifiedWrite N file - writes the contents of the given file into the
 *     selected device, verifying the writes as it does.
 */

const flashprog::args::argListDevices_t defaultOperation{};

int32_t main(int argCount, char **argList)
{
	console = {stdout, stderr};
	if (!parseArguments(argCount, argList, flashprog::options))
	{
		console.error("Failed to parse arguments"sv);
		return 1;
	}
	else if (args->ensureMaybeOneOf(argType_t::version, argType_t::help) == ensure_t::many)
	{
		console.error("Can only specify one of --help and --version, not both."sv);
		return 1;
	}
	else if (args->find(argType_t::version))
		return flashprog::versionInfo::printVersion();
	else if (args->find(argType_t::help))
		return flashprog::printHelp();
	else if (args->ensureMaybeOneOf(argType_t::listDevices, argType_t::list, argType_t::erase,
		argType_t::read, argType_t::write, argType_t::verifiedWrite) == ensure_t::many)
	{
		console.error("Multiple operations specified, please specify only one of "
			"listDevices, list, erase, read, write, and verifiedWrite only"sv);
		return 1;
	}

	const auto *operation{args->findAny(argType_t::listDevices, argType_t::list, argType_t::erase,
		argType_t::read, argType_t::write, argType_t::verifiedWrite)};
	if (!operation)
		operation = &defaultOperation;

	usbContext_t context{};

	if (!context.valid())
		return 2;

	std::vector<usbDevice_t> devices{};

	for (auto device : context.deviceList())
	{
		if (device.vid() == 0x1209 && device.pid() == 0xAB0C)
		{
			console.info("Found programmer at address "sv, device.busNumber(), ':', device.portNumber());
			devices.emplace_back(std::move(device));
		}
	}
	console.info("Found "sv, devices.size(), " programmers"sv);

	if (devices.size() == 1)
	{
		if (operation->type() == argType_t::listDevices)
			return listDevices(devices[0]);
		else if (operation->type() == argType_t::erase)
			return eraseDevice(devices[0], dynamic_cast<const argsTree_t *>(operation));
		else if (operation->type() == argType_t::read)
			return readDevice(devices[0], dynamic_cast<const argsTree_t *>(operation));
		else if (operation->type() == argType_t::write)
			return writeDevice(devices[0], dynamic_cast<const argsTree_t *>(operation));
	}

	return 0;
}
