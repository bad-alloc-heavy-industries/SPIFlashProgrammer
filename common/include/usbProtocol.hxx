// SPDX-License-Identifier: BSD-3-Clause
#ifndef USB_PROTOCOL__HXX
#define USB_PROTOCOL__HXX

#include <cstdint>
#include <array>

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
		external
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

	namespace requests
	{
		struct deviceCount_t final
		{
			messages_t type{messages_t::deviceCount};
		};

		struct listDevice_t final
		{
			messages_t type{messages_t::listDevice};
			uint8_t deviceNumber{0};
			deviceType_t deviceType;
		};

		struct targetDevice_t final
		{
			messages_t type{messages_t::targetDevice};
			uint8_t deviceNumber{0};
			deviceType_t deviceType;
		};

		struct erase_t final
		{
			messages_t type{messages_t::erase};
			eraseOperation_t operation{eraseOperation_t::status};
			// Only valid for eraseOperation_t::page{,Range}
			page_t beginPage{};
			// Only valid for eraseOperation_t::pageRange
			page_t endPage{};
		};

		struct read_t final
		{
			messages_t type{messages_t::read};
			page_t page{};
		};

		// This write_t is then followed by 64-byte blocks of data
		// Which contsitute the new contents of the page being written.
		struct write_t final
		{
			messages_t type{messages_t::write};
			page_t page{};
			page_t count{};
		};

		struct verify_t final
		{
			messages_t type{messages_t::verify};
		};

		struct resetTarget_t final
		{
			messages_t type{messages_t::resetTarget};
		};
	} // namespace requests

	namespace responses
	{
		struct deviceCount_t final
		{
			messages_t type{messages_t::deviceCount};
			uint8_t internalCount{0};
			uint8_t externalCount{0};
		};

		struct listDevice_t final
		{
			messages_t type{messages_t::listDevice};
			uint32_t deviceSize{0};
			uint8_t manufacturer{0};
			uint8_t deviceType{0};
			page_t pageSize{};
			page_t eraseSize{};
		};

		struct targetDevice_t final
		{
			messages_t type{messages_t::targetDevice};
		};

		struct erase_t final
		{
			messages_t type{messages_t::erase};
			uint8_t complete{0};
			page_t currentPage{};
		};

		// This read_t is then followed by 64-byte blocks of data
		// Which are the contents of the page being read and
		// constitute listDevice_t::pageSize of data.
		struct read_t final
		{
			messages_t type{messages_t::targetDevice};
		};

		struct write_t final
		{
			messages_t type{messages_t::write};
		};

		struct verify_t final
		{
			messages_t type{messages_t::verify};
			bool_t pageOk{false};
		};
	} // namespace responses
} // namespace flashProto

#endif /*USB_PROTOCOL__HXX*/
