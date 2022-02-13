// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_PROTOCOL__HXX
#define USB_PROTOCOL__HXX

#include <cstdint>
#include <array>

#ifdef __arm__
#include <cstring>
#include <type_traits>
#else
#include <exception>
#include "usbContext.hxx"
#endif

namespace flashProto
{
	enum class messages_t : uint8_t
	{
		deviceCount,
		listDevice,
		targetDevice,
		erase,
		read,
		write,
		verifiedWrite,
		resetTarget,
		status,
		abort
	};

	enum class flashBus_t : uint8_t
	{
		internal,
		external,
		unknown
	};

	enum class eraseOperation_t : uint8_t
	{
		all,
		page,
		pageRange,
		idle
	};

	struct page_t final
	{
	private:
		std::array<uint8_t, 3> value{};

	public:
		constexpr page_t() noexcept = default;
		constexpr page_t(const uint32_t page) noexcept :
			value{uint8_t(page), uint8_t(page >> 8U), uint8_t(page >> 16U)} { }

		constexpr operator uint32_t() const noexcept
			{ return value[0] | (uint32_t(value[1]) << 8U) | (uint32_t(value[2] << 16U)); }

		constexpr void operator ++() noexcept
		{
			const uint32_t page{*this};
			*this = page_t{page + 1};
		}
	};

	struct bool_t final
	{
	private:
		uint8_t value{};

	public:
		constexpr bool_t() noexcept = default;
		constexpr bool_t(const bool value_) noexcept : value{uint8_t(value_ ? 1U : 0U)} { }
		constexpr operator bool() const noexcept { return value; }
	};

	namespace responses
	{
#ifndef __arm__
		struct usbError_t final : std::exception
		{
			[[nodiscard]] const char *what() const noexcept final
				{ return "Failure during reading data from the device"; }
		};
#endif

		struct deviceCount_t final
		{
			messages_t type{messages_t::deviceCount};
			uint8_t internalCount{0};
			uint8_t externalCount{0};

			constexpr deviceCount_t() noexcept = default;

#ifndef __arm__
			deviceCount_t(const usbDeviceHandle_t &device, uint8_t endpoint) : deviceCount_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(deviceCount_t)); }
#endif
		};

		struct listDevice_t final
		{
			messages_t type{messages_t::listDevice};
			uint8_t manufacturer{0};
			uint8_t deviceType{0};
			uint32_t deviceSize{0};
			page_t pageSize{};
			page_t eraseSize{};

			constexpr listDevice_t() noexcept = default;

#ifndef __arm__
			listDevice_t(const usbDeviceHandle_t &device, uint8_t endpoint) : listDevice_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(listDevice_t)); }
#endif
		};

		struct erase_t final
		{
			messages_t type{messages_t::erase};
			uint8_t complete{0};
			page_t currentPage{};

			constexpr erase_t() noexcept = default;

#ifndef __arm__
			erase_t(const usbDeviceHandle_t &device, uint8_t endpoint) : erase_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(erase_t)); }
#endif
		};

		struct write_t final
		{
			messages_t type{messages_t::write};

			constexpr write_t() noexcept = default;

#ifndef __arm__
			write_t(const usbDeviceHandle_t &device, uint8_t endpoint) : write_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(write_t)); }
#endif
		};

		struct status_t final
		{
			uint8_t eraseComplete{};
			page_t erasePage{};
			bool writeOK{};
		};

		static_assert(sizeof(deviceCount_t) == 3);
		static_assert(sizeof(listDevice_t) == 16);
		static_assert(sizeof(erase_t) == 5);
		static_assert(sizeof(status_t) == 5);
	} // namespace responses

	namespace requests
	{
		namespace impl
		{
			struct address_t final
			{
				uint8_t addrL;
				uint8_t addrH;
			};

			static_assert(sizeof(address_t) == sizeof(uint16_t));
		} // namespace impl

#ifndef __arm__
		struct usbError_t final : std::exception
		{
			[[nodiscard]] const char *what() const noexcept final
				{ return "Failure during writing data to the device"; }
		};
#endif

		struct deviceCount_t final
		{
			constexpr deviceCount_t() noexcept = default;

#ifndef __arm__
			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t interface,
				responses::deviceCount_t &count) const noexcept
			{
				return device.readControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::deviceCount), 0, interface, count);
			}
#endif
		};

		struct listDevice_t final
		{
		public:
			uint8_t deviceNumber{0};
			flashBus_t deviceType{flashBus_t::unknown};

			constexpr listDevice_t() noexcept = default;
			constexpr listDevice_t(const uint8_t number, const flashBus_t type) noexcept :
				deviceNumber{number}, deviceType{type} { }

#ifndef __arm__
			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t interface,
				responses::listDevice_t &listing) const noexcept
			{
				impl::address_t address{deviceNumber, static_cast<uint8_t>(deviceType)};
				uint16_t index{};
				std::memcpy(&index, &address, sizeof(uint16_t));
				return device.readControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::listDevice), index, interface, listing);
			}
#endif

		private:
		};

		struct targetDevice_t final
		{
			uint8_t deviceNumber{0};
			flashBus_t deviceType{flashBus_t::unknown};

			constexpr targetDevice_t() noexcept = default;
			constexpr targetDevice_t(const uint8_t number, const flashBus_t type) noexcept :
				deviceNumber{number}, deviceType{type} { }

#ifndef __arm__
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t interface) const noexcept
			{
				impl::address_t address{deviceNumber, static_cast<uint8_t>(deviceType)};
				uint16_t index{};
				std::memcpy(&index, &address, sizeof(uint16_t));
				return device.writeControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::targetDevice), index, interface, nullptr);
			}
#endif
		};

		struct erase_t final
		{
			// Only valid for eraseOperation_t::page{,Range}
			page_t beginPage{};
			// Only valid for eraseOperation_t::pageRange
			page_t endPage{};

			constexpr erase_t() noexcept = default;
			constexpr erase_t(const page_t begin, const page_t end) noexcept :
				beginPage{begin}, endPage{end} { }

#ifndef __arm__
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t interface,
				const eraseOperation_t oper) const noexcept
			{
				uint16_t index{};
				static_assert(sizeof(eraseOperation_t) == sizeof(uint8_t));
				std::memcpy(&index, &oper, sizeof(uint8_t));
				return device.writeControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::erase), index, interface, *this);
			}
#endif
		};

		struct read_t final
		{
			page_t page{};

			constexpr read_t() noexcept = default;
			constexpr read_t(const page_t pageNumber) noexcept : page{pageNumber} { }

#ifndef __arm__
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t interface,
				const uint16_t readCount = 0) const noexcept
			{
				return device.writeControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::read), readCount, interface, page);
			}
#endif
		};

		// This write_t is then followed by 64-byte blocks of data
		// Which contsitute the new contents of the page being written.
		struct write_t final
		{
			bool verify{false};
			page_t page{};

			constexpr write_t() noexcept = default;
			constexpr write_t(const page_t pageNumber, bool verifyWrite = false) noexcept :
				verify{verifyWrite}, page{pageNumber} { }

#ifndef __arm__
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t interface,
				const uint16_t writeCount = 0) const noexcept
			{
				const auto request{verify ? messages_t::verifiedWrite : messages_t::write};
				return device.writeControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(request), writeCount, interface, page);
			}
#endif
		};

		struct resetTarget_t final
		{
			messages_t type{messages_t::resetTarget};
		};

		struct status_t final
		{
#ifndef __arm__
			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t interface,
				responses::status_t &status) const noexcept
			{
				return device.readControl({recipient_t::interface, request_t::typeClass},
					static_cast<uint8_t>(messages_t::status), 0, interface, status);
			}
#endif
		};

		static_assert(sizeof(deviceCount_t) == 1);
		static_assert(sizeof(listDevice_t) == 2);
		static_assert(sizeof(targetDevice_t) == 2);
		static_assert(sizeof(erase_t) == 6);
		static_assert(sizeof(read_t) == 3);
		static_assert(sizeof(write_t) == 4);
	} // namespace requests
} // namespace flashProto

#endif /*USB_PROTOCOL__HXX*/
