// SPDX-License-Identifier: BSD-3-Clause
#include <vector>
#include <thread>
#include <chrono>
#include <tuple>
#include <string_view>
#include <stdexcept>
#include <filesystem>
#include <substrate/utility>
#include <substrate/units>
#include <substrate/console>
#include <substrate/fd>
#include <substrate/command_line/arguments>
#include <version.hxx>
#include "options.hxx"
#include "help.hxx"
#include "usbContext.hxx"
#include "usbProtocol.hxx"
#include "sfdp.hxx"
#include "progress.hxx"
#include "utils/units.hxx"

// TODO: Add ChaiScript support for the flashing algorithms.

namespace flashprog
{
	inline int32_t printHelp() noexcept
	{
		console.info(helpString);
		return 0;
	}
} // namespace flashprog

using namespace flashProto;
using namespace std::literals::chrono_literals;
using namespace substrate;
using substrate::commandLine::arguments_t;
using substrate::commandLine::flag_t;
using substrate::commandLine::choice_t;
using flashprog::chip_t;

constexpr static auto transferBlockSize{4_KiB};
static arguments_t args{};

auto requestCount(const usbDeviceHandle_t &device)
{
	responses::deviceCount_t deviceCount{};
	if (!requests::deviceCount_t{}.read(device, 0, deviceCount))
		throw requests::usbError_t{};
	return std::make_tuple(deviceCount.internalCount, deviceCount.externalCount);
}

responses::listDevice_t readChipInfo(const usbDeviceHandle_t &device, const chip_t &chip)
{
	responses::listDevice_t chipInfo{};
	try
	{
		requests::listDevice_t request{chip.index, chip.bus};
		if (!request.read(device, 0, chipInfo))
			throw responses::usbError_t{};
	}
	catch (const responses::usbError_t &error)
	{
		console.error("Error getting target Flash chip information: "sv, error.what());
		if (!device.releaseInterface(0))
			throw std::nested_exception{};
		throw error;
	}
	return chipInfo;
}

bool listDevice(const usbDeviceHandle_t &device, const chip_t &chip) noexcept
{
	try
	{
		const auto chipInfo{readChipInfo(device, chip)};
		console.info('\t', chip.index, ": Manufacturer - "sv, chipInfo.manufacturer,
			", Capacity - "sv, chipInfo.deviceSize, ", Page size - "sv, uint32_t{chipInfo.pageSize},
			", Erase page size - "sv, uint32_t{chipInfo.eraseSize});
		return true;
	}
	catch (std::exception &)
		{ return false; }
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
		for (uint8_t internalDevice{0}; internalDevice < internalDeviceCount; ++internalDevice)
			listDevice(device, {flashBus_t::internal, internalDevice});
		if (externalDeviceCount)
			console.info("External devices:"sv);
		for (uint8_t externalDevice{0}; externalDevice < externalDeviceCount; ++externalDevice)
			listDevice(device, {flashBus_t::external, externalDevice});
	}
	catch (const requests::usbError_t &error)
	{
		console.error("Failure while listing devices: "sv, error.what());
		[[maybe_unused]] const auto result{device.releaseInterface(0)};
		return 1;
	}

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

bool targetDevice(const usbDeviceHandle_t &device, const flashBus_t deviceType, const uint8_t deviceNumber) noexcept
	{ return requests::targetDevice_t{deviceNumber, deviceType}.write(device, 0); }

void displayChipSize(const uint32_t chipSize) noexcept
{
	const auto [size, units] = flashprog::utils::humanReadableSize(chipSize);
	console.info("Chip is "sv, size, units, " in size"sv);
}

int32_t eraseDevice(const usbDevice_t &rawDevice, const arguments_t &eraseArgs)
{
	const auto &chip{std::any_cast<chip_t>(std::get<flag_t>(*eraseArgs["chip"sv]).value())};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	if (!requests::abort_t{}.write(device, 0) ||
		!targetDevice(device, chip.bus, chip.index))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	progressBar_t progress{"Erasing chip"sv};
	progress.display();
	const auto startTime{std::chrono::steady_clock::now()};

	if (!requests::erase_t{}.write(device, 0, eraseOperation_t::all))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}
	++progress;

	responses::status_t status{};
	while (!status.eraseComplete)
	{
		std::this_thread::sleep_for(125ms);
		if (!requests::status_t{}.read(device, 0, status))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		++progress;
	}
	progress.close();
	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	// This deselects the device
	if (!targetDevice(device, flashBus_t::unknown, 0))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!device.releaseInterface(0))
		return 1;
	// As complete is non-0 at this point, if it has the value 1, it was a good completion
	// otherwise, the programmer said we asked it to do something insane, so error out.
	return status.eraseComplete == 1 ? 0 : 1;
}

[[nodiscard]] int32_t readNormalDevice(const usbDeviceHandle_t &device, const responses::listDevice_t &chipInfo,
	substrate::fd_t &file)
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
	progressBar_t progress{"Reading chip "sv, blockCount};
	progress.display();
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
			!file.write(data))
		{
			console.error("Failed to read pages "sv, page, ":"sv, page + pagesPerBlock - 1,
				" back from the device"sv);
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		++progress;
	}
	progress.close();
	return 0;
}

[[nodiscard]] int32_t readTinyDevice(const usbDeviceHandle_t &device, const responses::listDevice_t &chipInfo,
	substrate::fd_t &file)
{
	const uint32_t pageSize{chipInfo.pageSize};
	const uint32_t pageCount{chipInfo.deviceSize / pageSize};
	progressBar_t progress{"Reading chip "sv, pageCount};
	// NOLINTNEXTLINE: cppcoreguidelines-avoid-c-arrays
	auto data{std::make_unique<std::byte []>(pageSize)};
	progress.display();
	for (uint32_t page{}; page < pageCount; ++page)
	{
		if (!requests::read_t{page}.write(device, 0))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}

		if (!device.readBulk(1, data.get(), static_cast<int32_t>(pageSize)) ||
			!file.write(data, pageSize))
		{
			console.error("Failed to read page "sv, page, " back from the device"sv);
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		++progress;
	}
	progress.close();
	return 0;
}

int32_t readDevice(const usbDevice_t &rawDevice, const arguments_t &readArgs)
{
	const auto &chip{std::any_cast<chip_t>(std::get<flag_t>(*readArgs["chip"sv]).value())};
	const auto &fileName
	{
		[](const commandLine::item_t *arg)
		{
			if (!arg)
				throw std::logic_error{"File name to write data read from the device is null"};
			return std::any_cast<std::filesystem::path>(std::get<flag_t>(*arg).value());
		}(readArgs["file"sv])
	};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	substrate::fd_t file{fileName, O_CREAT | O_WRONLY | O_NOCTTY, substrate::normalMode};
	if (!file.valid())
	{
		console.error("Failed to open output file '"sv, fileName.u8string(), "'"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	const auto chipInfo{readChipInfo(device, chip)};
	if (!requests::abort_t{}.write(device, 0) ||
		!targetDevice(device, chip.bus, chip.index))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	displayChipSize(chipInfo.deviceSize);
	const auto startTime{std::chrono::steady_clock::now()};
	const auto result
	{
		[&]()
		{
			if (chipInfo.deviceSize >= transferBlockSize)
				return readNormalDevice(device, chipInfo, file);
			else
				return readTinyDevice(device, chipInfo, file);
		}()
	};
	if (result != 0)
		return result;
	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	// This deselects the device
	if (!targetDevice(device, flashBus_t::unknown, 0))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}

int32_t erasePages(const usbDeviceHandle_t &device, const responses::listDevice_t chipInfo, size_t fileLength)
{
	responses::status_t status{};
	const uint32_t pageSize{chipInfo.eraseSize};
	const uint32_t pageCount
	{
		[fileLength, pageSize] () -> uint32_t
		{
			const auto pages{fileLength / pageSize};
			const auto remainder{fileLength % pageSize};
			return pages + (remainder ? 1U : 0U);
		}()
	};

	progressBar_t progress{"Erasing chip "sv, pageCount};
	progress.display();
	if (!requests::erase_t{{}, {pageCount}}.write(device, 0, eraseOperation_t::pageRange))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	page_t currentPage{};
	while (!status.eraseComplete)
	{
		std::this_thread::sleep_for(10ms);
		if (!requests::status_t{}.read(device, 0, status))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		if (currentPage != status.erasePage)
		{
			progress += status.erasePage - currentPage;
			currentPage = status.erasePage;
		}
		else
			progress.display();
	}
	progress.close();
	return 0;
}

[[nodiscard]] int32_t verifyPages(const usbDeviceHandle_t &device, const uint32_t pagesPerBlock, const uint32_t page)
{
	responses::status_t status{};
	// Read back the programmer status
	if (!requests::status_t{}.read(device, 0, status))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}
	// Then check the verification status
	if (!status.writeOK)
	{
		console.error("Verification of data on pages "sv, page, ":"sv,
			page + pagesPerBlock - 1, " failed"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}
	return 0;
}

[[nodiscard]] int32_t writeNormalDevice(const usbDeviceHandle_t &device, const responses::listDevice_t &chipInfo,
	const substrate::fd_t &file, const substrate::off_t fileLength, const bool verify)
{
	if (chipInfo.deviceSize % transferBlockSize)
	{
		console.error("Funky device size, is "sv, chipInfo.deviceSize,
			", was expecting a device size that divided by "sv, transferBlockSize);
		if (!device.releaseInterface(0))
			return 2;
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
	progressBar_t progress{"Writing chip "sv, blockCount};
	progress.display();
	for (uint32_t block{}; block < blockCount; ++block)
	{
		const auto page{block * pagesPerBlock};
		const auto byteCount{std::min(uint32_t(fileLength) - (block * transferBlockSize), transferBlockSize)};
		if (!requests::write_t{page, verify}.write(device, 0, byteCount))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}

		std::array<std::byte, transferBlockSize> data{};
		if (!file.read(data.data(), byteCount) ||
			!device.writeBulk(1, data.data(), static_cast<int32_t>(byteCount)))
		{
			console.error("Failed to write pages "sv, page, ":"sv, page + pagesPerBlock - 1,
				" to the device"sv);
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		if (verify)
		{
			const auto result{verifyPages(device, pagesPerBlock, page)};
			if (result != 0)
				return result;
		}
		++progress;
	}
	progress.close();
	return 0;
}

[[nodiscard]] int32_t writeTinyDevice(const usbDeviceHandle_t &device, const responses::listDevice_t &chipInfo,
	const substrate::fd_t &file, const substrate::off_t fileLength, [[maybe_unused]] const bool verify)
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
	progressBar_t progress{"Writing chip "sv, pageCount};
	// NOLINTNEXTLINE: cppcoreguidelines-avoid-c-arrays
	auto data{std::make_unique<std::byte []>(pageSize)};
	progress.display();
	for (uint32_t page{}; page < pageCount; ++page)
	{
		const auto byteCount{std::min(uint32_t(fileLength) - (page * pageSize), pageSize)};
		if (!requests::write_t{page}.write(device, 0, byteCount))
		{
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}

		if (!file.read(data, byteCount) ||
			!device.writeBulk(1, data.get(), static_cast<int32_t>(byteCount)))
		{
			console.error("Failed to write page "sv, page, " to the device"sv);
			if (!device.releaseInterface(0))
				return 2;
			return 1;
		}
		// XXX: We need to write the veriication step for tiny devices still
		++progress;
	}
	progress.close();
	return 0;
}

int32_t writeDevice(const usbDevice_t &rawDevice, const arguments_t &writeArgs, const bool verify)
{
	const auto &chip{std::any_cast<chip_t>(std::get<flag_t>(*writeArgs["chip"sv]).value())};
	const auto &fileName
	{
		[](const commandLine::item_t *arg)
		{
			if (!arg)
				throw std::logic_error{"File name to consume data from to write to device is null"};
			return std::any_cast<std::filesystem::path>(std::get<flag_t>(*arg).value());
		}(writeArgs["file"sv])
	};

	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	const substrate::fd_t file{fileName, O_RDONLY | O_NOCTTY};
	if (!file.valid())
	{
		console.error("Failed to open input file '"sv, fileName.u8string(), "'"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	const auto chipInfo{readChipInfo(device, chip)};
	const auto fileLength{file.length()};
	if (fileLength < 0 || fileLength > chipInfo.deviceSize)
	{
		console.error("The file given is larger than the target device"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!requests::abort_t{}.write(device, 0) ||
		!targetDevice(device, chip.bus, chip.index))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	displayChipSize(chipInfo.deviceSize);
	const auto startTime{std::chrono::steady_clock::now()};
	const auto eraseResult{erasePages(device, chipInfo, fileLength)};
	if (eraseResult)
		return eraseResult;

	const auto result
	{
		[&]()
		{
			if (chipInfo.deviceSize >= transferBlockSize)
				return writeNormalDevice(device, chipInfo, file, fileLength, verify);
			else
				return writeTinyDevice(device, chipInfo, file, fileLength, verify);
		}()
	};
	if (result != 0)
		return result;

	const auto endTime{std::chrono::steady_clock::now()};

	console.info("Complete"sv);
	const auto elapsedSeconds{std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime)};
	console.info("Total time elapsed: "sv, substrate::asTime_t{uint64_t(elapsedSeconds.count())});

	// This deselects the device
	if (!targetDevice(device, flashBus_t::unknown, 0))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	if (!device.releaseInterface(0))
		return 1;
	return 0;
}


int32_t dumpSFDP(const usbDevice_t &rawDevice, const arguments_t &sfdpArgs)
{
	const auto &chip{std::any_cast<chip_t>(std::get<flag_t>(*sfdpArgs["chip"sv]).value())};

	// Setup the USB interface for use
	const auto device{rawDevice.open()};
	if (!device.valid() ||
		!device.claimInterface(0))
		return 1;

	// Abort any stale running command and select the requested Flash chip
	if (!requests::abort_t{}.write(device, 0) ||
		!targetDevice(device, chip.bus, chip.index))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	// Call into the PC-side SFDP reader and display engine
	if (!sfdp::readAndDisplay(device, {0, 1}))
	{
		console.error("Failed to read SFDP data"sv);
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	// Deselect the Flash chip now we're complete
	if (!targetDevice(device, flashBus_t::unknown, 0))
	{
		if (!device.releaseInterface(0))
			return 2;
		return 1;
	}

	// And clean up
	return device.releaseInterface(0) ? 0 : 1;
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
 * sfdp N - Dump the SFDP data for the given device
 */

const static commandLine::item_t defaultOperation{commandLine::choice_t{"action"sv, "listDevices"sv, {}}};

int main(const int argCount, const char *const *const argList) noexcept
{
	console = {stdout, stderr};
	if (const auto parsedArgs{parseArguments(argCount, argList, flashprog::programOptions)}; !parsedArgs)
	{
		console.error("Failed to parse arguments"sv);
		return 1;
	}
	else
		args = *parsedArgs;
	const auto &version{args.find("version"sv)};
	const auto &help{args.find("help"sv)};
	if (version != args.end() && help != args.end())
	{
		console.error("Can only specify one of --help and --version, not both."sv);
		return 1;
	}
	if (version != args.end())
		return flashprog::versionInfo::printVersion();
	if (help != args.end())
		return flashprog::printHelp();

	const auto *operation{args["action"sv]};
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
		const auto &operationArg{std::get<choice_t>(*operation)};
		if (operationArg.value() == "listDevices"sv)
			return listDevices(devices[0]);
		if (operationArg.value() == "erase"sv)
			return eraseDevice(devices[0], operationArg.arguments());
		if (operationArg.value() == "read"sv)
			return readDevice(devices[0], operationArg.arguments());
		if (operationArg.value() == "write"sv)
			return writeDevice(devices[0], operationArg.arguments(), false);
		if (operationArg.value() == "verifiedWrite"sv)
			return writeDevice(devices[0], operationArg.arguments(), true);
		if (operationArg.value() == "sfdp"sv)
			return dumpSFDP(devices[0], operationArg.arguments());
	}

	return 0;
}
