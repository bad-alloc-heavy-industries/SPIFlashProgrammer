// SPDX-License-Identifier: BSD-3-Clause
#ifndef PROGRESS__HXX
#define PROGRESS__HXX

#include <cstddef>
#include <string_view>

struct progressBar_t final
{
private:
	std::size_t count_{};
	std::size_t total_;
	std::size_t rows_{};
	std::size_t cols_{};
	std::string_view prefix_{};
	bool disable{false};

public:
	progressBar_t(std::string_view prefix, std::size_t total) noexcept;
	progressBar_t(const progressBar_t &) = delete;
	progressBar_t(progressBar_t &&) = default;
	progressBar_t &operator =(const progressBar_t &) = delete;
	progressBar_t &operator =(progressBar_t &&) = default;
	~progressBar_t() noexcept { close(); }

	progressBar_t &operator +=(std::size_t amount) noexcept;
	progressBar_t &operator ++() noexcept { return *this += 1; }
	void update(const std::size_t amount) noexcept { *this += amount; }
	void updateWindowSize() noexcept;
	void display() const noexcept;
	void close() noexcept;
};

#endif /*PROGRESS__HXX*/
