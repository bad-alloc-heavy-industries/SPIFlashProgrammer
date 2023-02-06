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
#include <fmt/core.h>
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

constexpr static std::array<char, 4> percentageUnknown{"---"};

constexpr static auto spinnerTimestep{75ms};

// Handler for console window size changes
void sigwinchHandler(const int32_t) noexcept
{
	if (currentProgressBar)
		currentProgressBar->updateWindowSize();
}

// Construct a new progress bar with a descriptive string prefix and optionally some
// total amount of progress to count up to
progressBar_t::progressBar_t(std::string_view prefix, std::optional<std::size_t> total) noexcept :
	total_{total}, prefix_{prefix}
{
	struct sigaction action{};
	action.sa_flags = SA_RESTART;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
	action.sa_handler = sigwinchHandler;
	sigaction(SIGWINCH, &action, nullptr);
	updateWindowSize();
	currentProgressBar = this;
	// If we do not have a valid total, capture the construction time as when the progress indication was started
	if (!total_)
		startTime_ = std::make_optional(std::chrono::steady_clock::now());
}

void progressBar_t::updateWindowSize() noexcept
{
	// Grab the new console width/height if possible
	winsize result{};
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &result) < 0)
	{
		// If we cannot, read them from the COLUMNS and LINES environment variables
		const auto *const columns{getenv("COLUMNS")};
		const auto *const rows{getenv("LINES")};
		// Assuming we got valid strings (nullptr indicates they weren't found in our environment block)
		if (columns && rows)
		{
			// Convert them to numbers safely
			rows_ = substrate::toInt_t<std::size_t>{rows};
			cols_ = substrate::toInt_t<std::size_t>{columns} - 7;
		}
	}
	else
	{
		// We got the console size with the ioctl
		rows_ = result.ws_row;
		cols_ = result.ws_col - 7;
	}
}

// Update the progress indication by some amount and redisplay the bar
progressBar_t &progressBar_t::operator +=(std::size_t amount) noexcept
{
	count_ += amount;
	display();
	return *this;
}

// Helper for building the actual progress bar graphic in the console
struct bar_t final
{
private:
	float fraction;
	std::size_t length;

	// This is a series of Unicode characters that provide an extremely smooth bar
	constexpr static auto charset
	{
		substrate::make_array(
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
		})
	};

public:
	bar_t(const float frac, const std::size_t cols) : fraction{frac}, length{cols} { }

	// Convert the bar state into the progress bar string
	operator std::string() const noexcept
	{
		const auto charsetSyms{charset.size() - 1U};
		// Figure out how many full blocks to display and how wide the fractional block is
		const auto [barLength, fractionalBarIndex] =
			std::div(int64_t(fraction * static_cast<float>(length * charsetSyms)), charsetSyms);
		std::string result{};
		// Fill the full block component of the string with the full block character
		//std::string result{charset.back(), 0U, std::size_t(barLength)};
		for (std::size_t i{}; i < std::size_t(barLength); ++i)
			result += charset.back();
		if (std::size_t(barLength) < length)
		{
			// Now if there's space to go, deal with the fractional component
			result += charset[fractionalBarIndex];
			// And fill the remaining space with the empty (space) block character
			//result += std::string{charset[0], 0U, std::size_t{length - barLength - 1U}};
			for (std::size_t i{}; i < std::size_t{length - barLength - 1U}; ++i)
				result += charset[0];
		}
		return result;
	}
};

void progressBar_t::display() noexcept
{
	// Turn the progress indication into fraction and reserve space for the stringification of that fraction
	const auto frac{float(count_) / static_cast<float>(total_ ? *total_ : 1)};
	std::array<char, 4> percentageBuffer{};
	if (total_)
	{
		fmt::format_to_n(percentageBuffer.begin(), percentageBuffer.size(), "{:>3.0f}"sv, frac * 100);
		// We only have to NUL terminate because format_to_n doesn't guarantee to.
		percentageBuffer[percentageBuffer.size() - 1] = 0;
	}
	else
		percentageBuffer = percentageUnknown;
	// Having built the progress indication, turn it into a string_view for use in the string handling below
	const std::string_view percentage{percentageBuffer.data(), percentageBuffer.size()};

	// Compute how long the progress bar needs to be in characters
	const auto barLength{prefix_.size() + prefixSeperator.size() + percentage.size() +
		percentageSeperator.size() + endSeperator.size() + 1U};
	// Create a suitable progress bar to display
	std::string bar(
		total_ ?
			bar_t{frac, std::max<std::size_t>(1U, cols_ - barLength)} :
			bar_t{0.f, std::max<std::size_t>(1U, cols_ - barLength)}
	);

	// Determine the current end-of-bar spinner character and update the index based on the the time since last step
	const auto spinnerChar{spinner[spinnerStep_]};
	const auto now{std::chrono::steady_clock::now()};
	if (now - spinnerLastUpdated_ >= spinnerTimestep)
	{
		spinnerStep_ = (spinnerStep_ + 1) % spinner.size();
		spinnerLastUpdated_ = now;
	}

	// If the total progress target is unknown (and the start time stored as a result is valid)
	if (!total_ && startTime_)
	{
		// Figure out how long has elapsed since we started
		const auto value{std::chrono::duration_cast<std::chrono::seconds>(now - *startTime_).count()};
		const auto minsValue{value / 60U};
		const auto secsValue{value % 60U};
		// Convert the resulting number of minutes and seconds to strings
		const substrate::fromInt_t mins{minsValue};
		const substrate::fromInt_t secs{secsValue};
		const auto totalDigits{mins.digits() + secs.digits() + 3U};

		// If there's sufficient space to display the result, format it out
		if (totalDigits < barLength)
		{
			size_t offset{(cols_ - barLength - totalDigits) / 2U};
			// First deal with inserting the minutes count in the progress bar space
			mins.formatTo(bar.data() + offset);
			offset += mins.digits();
			bar[offset] = 'm';
			offset += 2U;
			// Then the seconds count
			secs.formatTo(bar.data() + offset);
			offset += secs.digits();
			bar[offset] = 's';
		}
	}

	// Zip the cursor back to the start of the line and display the new progress bar
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
