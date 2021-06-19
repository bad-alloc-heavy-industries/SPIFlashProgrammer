// SPDX-License-Identifier: BSD-3-Clause
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <array>
#include <csignal>
#include <substrate/console>
#include <substrate/conversions>
#include "progress.hxx"

using substrate::console;
using namespace std::literals::string_view_literals;
using namespace std::literals::chrono_literals;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
progressBar_t *currentProgressBar{nullptr};

constexpr static auto prefixSeperator{": "sv};
constexpr static auto percentageSeperator{"% |"sv};
constexpr static auto endSeperator{"|"sv};

#define STYLE1

constexpr static auto spinner{substrate::make_array<std::string_view>(
{
#if defined(STYLE1)
	u8"\u283B"sv,
	u8"\u283D"sv,
	u8"\u283E"sv,
	u8"\u2837"sv,
	u8"\u282F"sv,
	u8"\u281F"sv
#elif defined(STYLE2)
	u8"\u2599"sv,
	u8"\u259b"sv,
	u8"\u259c"sv,
	u8"\u259f"sv
#elif defined(STYLE3)
	u8"\u25DC"sv,
	u8"\u25DD"sv,
	u8"\u25DE"sv,
	u8"\u25DF"sv
#elif defined(STYLE4)
	u8"\u2500"sv,
	u8"\u2572"sv,
	u8"\u2502"sv,
	u8"\u2571"sv,
#else
#error "Must define a valid spinner style"
#endif
})};

constexpr static auto spinnerTimestep{75ms};

void sigwinchHandler(const int32_t) noexcept
{
	if (currentProgressBar)
		currentProgressBar->updateWindowSize();
}

progressBar_t::progressBar_t(std::string_view prefix, const std::size_t total) noexcept :
	total_{total}, prefix_{prefix}
{
	struct sigaction action{};
	action.sa_flags = SA_RESTART;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
	action.sa_handler = sigwinchHandler;
	sigaction(SIGWINCH, &action, nullptr);
	updateWindowSize();
	currentProgressBar = this;
}

void progressBar_t::updateWindowSize() noexcept
{
	winsize result{};
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &result) < 0)
	{
		const auto columns{getenv("COLUMNS")};
		const auto rows{getenv("LINES")};
		if (columns && rows)
		{
			rows_ = substrate::toInt_t<std::size_t>{rows};
			cols_ = substrate::toInt_t<std::size_t>{columns} - 7;
		}
	}
	else
	{
		rows_ = result.ws_row;
		cols_ = result.ws_col - 7;
	}
}

progressBar_t &progressBar_t::operator +=(std::size_t amount) noexcept
{
	count_ += amount;
	display();
	return *this;
}

struct bar_t final
{
private:
	float fraction;
	std::size_t length;

	constexpr static auto charset{substrate::make_array<std::string_view>(
	{
		u8" "sv,
		u8"\u258F"sv,
		u8"\u258E"sv,
		u8"\u258D"sv,
		u8"\u258C"sv,
		u8"\u258B"sv,
		u8"\u258A"sv,
		u8"\u2589"sv,
		u8"\u2588"sv
	})};

public:
	bar_t(const float frac, const std::size_t cols) : fraction{frac}, length{cols} { }
	operator std::string() const noexcept
	{
		const auto charsetSyms{charset.size() - 1U};
		const auto [barLength, fractionalBarIndex] =
			std::div(int64_t(fraction * length * charsetSyms), charsetSyms);
		std::string result{};
		//std::string result{charset.back(), 0U, std::size_t(barLength)};
		for (std::size_t i{}; i < std::size_t(barLength); ++i)
			result += charset.back();
		if (std::size_t(barLength) < length)
		{
			result += charset[fractionalBarIndex];
			//result += std::string{charset[0], 0U, std::size_t{length - barLength - 1U}};
			for (std::size_t i{}; i < std::size_t{length - barLength - 1U}; ++i)
				result += charset[0];
		}
		return result;
	}
};

void progressBar_t::display() noexcept
{
	const auto frac{float(count_) / total_};
	std::array<char, 4> percentageBuffer{};
	std::snprintf(percentageBuffer.data(), percentageBuffer.size(), "%3.0f", frac * 100);
	const std::string_view percentage{percentageBuffer.data(), percentageBuffer.size()};

	const auto barLength{prefix_.size() + prefixSeperator.size() + percentage.size() +
		percentageSeperator.size() + endSeperator.size() + 1U};
	std::string bar(bar_t{frac, std::max<std::size_t>(1U, cols_ - barLength)});

	const auto spinnerChar{spinner[spinnerStep_]};
	const auto now{std::chrono::steady_clock::now()};
	if ((now - spinnerLastUpdated_) >= spinnerTimestep)
	{
		spinnerStep_ = (spinnerStep_ + 1) % spinner.size();
		spinnerLastUpdated_ = now;
	}

	console.writeln('\r', nullptr);
	console.info(prefix_, prefixSeperator, percentage, percentageSeperator, bar,
		endSeperator, spinnerChar, ' ', nullptr);
}

void progressBar_t::close() noexcept
{
	if (disable)
		return;
	disable = true;
	console.writeln();
	currentProgressBar = nullptr;
}
