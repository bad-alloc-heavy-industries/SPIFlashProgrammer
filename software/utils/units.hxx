// SPDX-License-Identifier: BSD-3-Clause
#ifndef UTILS_UNITS_HXX
#define UTILS_UNITS_HXX

#include <cstdint>
#include <tuple>
#include <string_view>

namespace flashprog::utils
{
	using namespace std::literals::string_view_literals;

	inline std::tuple<uint32_t, std::string_view> humanReadableSize(uint32_t size)
	{
		if (size < 1024U)
			return {size, "B"sv};
		size /= 1024U;
		if (size < 1024U)
			return {size, "kiB"sv};
		size /= 1024U;
		if (size < 1024U)
			return {size, "MiB"sv};
		size /= 1024U;
		return {size, "GiB"sv};
	}
} // namespace flashprog::utils

#endif /*UTILS_UNITS_HXX*/
