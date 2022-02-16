#ifndef INDEXED_ITERATOR_HXX
#define INDEXED_ITERATOR_HXX

#include <cstdint>
#include <cstddef>
#include <utility>

namespace utility
{
	template<typename T> struct indexedItem_t final
	{
	private:
		std::pair<std::size_t, T> item_;

	public:
		constexpr indexedItem_t(const std::size_t index, const T item) noexcept : item_{index, item} { }
		[[nodiscard]] constexpr auto index() const noexcept { return item_.first; }
		[[nodiscard]] constexpr auto item() const noexcept { return item_.second; }
		[[nodiscard]] constexpr auto &operator *() const noexcept { return item_; }

		constexpr auto operator ++() noexcept
		{
			++item_.first;
			++item_.second;
			return *this;
		}
	};

	template<typename T> indexedItem_t(std::size_t, T) -> indexedItem_t<T>;

	template<typename T>
		[[nodiscard]] constexpr inline auto operator ==(const indexedItem_t<T> &a, const indexedItem_t<T> &b) noexcept
			{ return a.index() == b.index(); }
	template<typename T>
		[[nodiscard]] constexpr inline auto operator !=(const indexedItem_t<T> &a, const indexedItem_t<T> &b) noexcept
			{ return a.index() != b.index(); }

	template<typename T> struct indexedIterator_t final
	{
	private:
		T &container_;

	public:
		constexpr indexedIterator_t(T &container) noexcept : container_{container} { }

		[[nodiscard]] constexpr auto begin() const noexcept
			{ return indexedItem_t{0, std::begin(container_)}; }
		[[nodiscard]] constexpr auto end() const noexcept
			{ return indexedItem_t{std::size(container_), std::end(container_)}; }
	};

	template<typename T> indexedIterator_t(T) -> indexedIterator_t<T>;
} // namespace utility

#endif /*INDEXED_ITERATOR_HXX*/
