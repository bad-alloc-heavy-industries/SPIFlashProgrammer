// SPDX-License-Identifier: BSD-3-Clause
#ifndef PROGRESS_HXX
#define PROGRESS_HXX

#include <cstddef>
#include <string_view>
#include <chrono>
#include <optional>

struct progressBar_t final
{
private:
	using time_t = typename std::chrono::steady_clock::time_point;

	std::size_t count_{};
	std::optional<std::size_t> total_;
	std::size_t rows_{};
	std::size_t cols_{};
	std::size_t spinnerStep_{};
	std::string_view prefix_{};
	time_t spinnerLastUpdated_{};
	bool disable{false};
	std::optional<time_t> startTime_{std::nullopt};

public:
	progressBar_t(std::string_view prefix, std::optional<std::size_t> total = std::nullopt) noexcept;
	progressBar_t(const progressBar_t &) = delete;
	progressBar_t(progressBar_t &&) = default;
	progressBar_t &operator =(const progressBar_t &) = delete;
	progressBar_t &operator =(progressBar_t &&) = default;
	~progressBar_t() noexcept { close(); }

	progressBar_t &operator +=(std::size_t amount) noexcept;
	progressBar_t &operator ++() noexcept { return *this += 1; }
	void update(const std::size_t amount) noexcept { *this += amount; }
	void updateWindowSize() noexcept;
	void display() noexcept;
	void close() noexcept;
};

#endif /*PROGRESS_HXX*/
