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
		verify,
		resetTarget
	};

	enum class deviceType_t : uint8_t
	{
		internal,
		external,
		none
	};

	enum class eraseOperation_t : uint8_t
	{
		all,
		page,
		pageRange,
		status
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

		struct targetDevice_t final
		{
			messages_t type{messages_t::targetDevice};

			constexpr targetDevice_t() noexcept = default;

#ifndef __arm__
			targetDevice_t(const usbDeviceHandle_t &device, uint8_t endpoint) : targetDevice_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(targetDevice_t)); }
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

		// This read_t is then followed by 64-byte blocks of data
		// Which are the contents of the page being read and
		// constitute listDevice_t::pageSize of data.
		struct read_t final
		{
			messages_t type{messages_t::read};

			constexpr read_t() noexcept = default;

#ifndef __arm__
			read_t(const usbDeviceHandle_t &device, uint8_t endpoint) : read_t{}
			{
				if (!read(device, endpoint))
					throw usbError_t{};
			}

			[[nodiscard]] bool read(const usbDeviceHandle_t &device, uint8_t endpoint) noexcept
				{ return device.readInterrupt(endpoint, &type, sizeof(read_t)); }
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

		struct verify_t final
		{
			messages_t type{messages_t::verify};
			bool_t pageOk{false};
		};

		static_assert(sizeof(deviceCount_t) == 3);
		static_assert(sizeof(listDevice_t) == 16);
		static_assert(sizeof(targetDevice_t) == 1);
		static_assert(sizeof(erase_t) == 5);
		static_assert(sizeof(read_t) == 1);
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
			deviceType_t deviceType{deviceType_t::none};

			constexpr listDevice_t() noexcept = default;
			constexpr listDevice_t(const uint8_t number, const deviceType_t type) noexcept :
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
			messages_t type{messages_t::targetDevice};
			uint8_t deviceNumber{0};
			deviceType_t deviceType{deviceType_t::none};

			constexpr targetDevice_t() noexcept = default;

#ifdef __arm__
			template<size_t N> targetDevice_t(const std::array<uint8_t, N> &data) noexcept : targetDevice_t{}
			{
				static_assert(N >= sizeof(targetDevice_t));
				std::memcpy(&type, data.data(), sizeof(targetDevice_t));
			}
#else
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t endpoint) const noexcept
				{ return device.writeInterrupt(endpoint, &type, sizeof(targetDevice_t)); }
#endif
		};

		struct erase_t final
		{
			messages_t type{messages_t::erase};
			eraseOperation_t operation{eraseOperation_t::status};
			// Only valid for eraseOperation_t::page{,Range}
			page_t beginPage{};
			// Only valid for eraseOperation_t::pageRange
			page_t endPage{};

			constexpr erase_t() noexcept = default;

#ifdef __arm__
			template<size_t N> erase_t(const std::array<uint8_t, N> &data) noexcept : erase_t{}
			{
				static_assert(N >= sizeof(erase_t));
				std::memcpy(&type, data.data(), sizeof(erase_t));
			}
#else
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t endpoint) const noexcept
				{ return device.writeInterrupt(endpoint, &type, sizeof(erase_t)); }
#endif
		};

		struct read_t final
		{
			messages_t type{messages_t::read};
			page_t page{};

			constexpr read_t() noexcept = default;

#ifdef __arm__
			template<size_t N> read_t(const std::array<uint8_t, N> &data) noexcept : read_t{}
			{
				static_assert(N >= sizeof(read_t));
				std::memcpy(&type, data.data(), sizeof(read_t));
			}
#else
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t endpoint) const noexcept
				{ return device.writeInterrupt(endpoint, &type, sizeof(read_t)); }
#endif
		};

		// This write_t is then followed by 64-byte blocks of data
		// Which contsitute the new contents of the page being written.
		struct write_t final
		{
			messages_t type{messages_t::write};
			page_t page{};
			page_t count{};

			constexpr write_t() noexcept = default;

#ifdef __arm__
			template<size_t N> write_t(const std::array<uint8_t, N> &data) noexcept : write_t{}
			{
				static_assert(N >= sizeof(write_t));
				std::memcpy(&type, data.data(), sizeof(write_t));
			}
#else
			[[nodiscard]] bool write(const usbDeviceHandle_t &device, uint8_t endpoint) const noexcept
				{ return device.writeInterrupt(endpoint, &type, sizeof(write_t)); }
#endif
		};

		struct verify_t final
		{
			messages_t type{messages_t::verify};
		};

		struct resetTarget_t final
		{
			messages_t type{messages_t::resetTarget};
		};

		static_assert(sizeof(deviceCount_t) == 1);
		static_assert(sizeof(listDevice_t) == 2);
		static_assert(sizeof(targetDevice_t) == 3);
		static_assert(sizeof(erase_t) == 8);
		static_assert(sizeof(read_t) == 4);
	} // namespace requests
} // namespace flashProto

#endif /*USB_PROTOCOL__HXX*/
