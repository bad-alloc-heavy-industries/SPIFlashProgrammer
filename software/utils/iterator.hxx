#ifndef UTILS_ITERATOR__HXX
#define UTILS_ITERATOR__HXX

#include <iterator>

namespace flashprog::utils
{
	using std::iterator_traits;
	using std::iterator;

	/**
	 * Defines an iterator adapter that is 'normal' in the sense that
	 * it does not change the semantics of any of the operators of its
	 * \c iterator_t parameter. Its primary purpose is to convert an
	 * iterator that is not a class (ie a pointer), into an iterator that is.
	 * \c container_t exists as a container-specific tag for instanciation
	 * differentiation purposes.
	 */
	template<typename iterator_t, typename container_t> struct normalIterator_t
	{
	protected:
		// NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
		iterator_t current{iterator_t{}};

		using traits_t = iterator_traits<iterator_t>;

	public:
		using iterator_type = iterator_t;
		using iterator_category = typename traits_t::iterator_category;
		using value_type = typename traits_t::value_type;
		using difference_type = typename traits_t::difference_type;
		using reference = typename traits_t::reference;
		using pointer = typename traits_t::pointer;

		constexpr normalIterator_t() noexcept = default;
		explicit normalIterator_t(const iterator_t &iter) noexcept : current{iter} { }

		// We specifically allow iterator_t to const iterator_t conversion.
		template<typename iter_t> normalIterator_t(const normalIterator_t<iter_t,
			std::enable_if_t<std::is_same_v<iter_t, typename container_t::pointer>,
			container_t>> &iter) noexcept : current{iter.base()} { }

		[[nodiscard]] reference operator *() const noexcept { return *current; }
		[[nodiscard]] pointer operator ->() const noexcept { return current; }
		const normalIterator_t operator++(int) const noexcept // NOLINT(readability-const-return-type)
			{ return normalIterator_t{current++}; }
		const normalIterator_t operator--(int) const noexcept // NOLINT(readability-const-return-type)
			{ return normalIterator_t{current--}; }

		normalIterator_t &operator ++() noexcept
		{
			++current;
			return *this;
		}

		normalIterator_t &operator --() noexcept
		{
			--current;
			return *this;
		}

		[[nodiscard]] reference operator [](const difference_type offset) const noexcept
			{ return current[offset]; }
		[[nodiscard]] normalIterator_t operator +(const difference_type offset) const noexcept
			{ return normalIterator_t{current + offset}; }
		[[nodiscard]] normalIterator_t operator -(const difference_type offset) const noexcept
			{ return normalIterator_t{current - offset}; }

		normalIterator_t operator +=(const difference_type offset) const noexcept
		{
			current += offset;
			return *this;
		}

		normalIterator_t operator -=(const difference_type offset) const noexcept
		{
			current -= offset;
			return *this;
		}

		[[nodiscard]] iterator_t base() const noexcept { return current; }
	};

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator ==(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() == rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator ==(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() == rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator !=(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() != rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator !=(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() != rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator <(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() < rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator <(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() < rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator >(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() > rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator >(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() > rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator <=(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() <= rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator <=(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() <= rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline bool operator >=(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept
		{ return lhs.base() >= rhs.base(); }

	template<typename iterator_t, typename container_t> inline bool operator >=(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept
		{ return lhs.base() >= rhs.base(); }

	template<typename iteratorL_t, typename iteratorR_t, typename container_t> inline auto operator -(
		const normalIterator_t<iteratorL_t, container_t> &lhs,
		const normalIterator_t<iteratorR_t, container_t> &rhs
	) noexcept -> decltype(lhs.base() - rhs.base())
		{ return lhs.base() - rhs.base(); }

	template<typename iterator_t, typename container_t> inline auto operator -(
		const normalIterator_t<iterator_t, container_t> &lhs,
		const normalIterator_t<iterator_t, container_t> &rhs
	) noexcept -> typename normalIterator_t<iterator_t, container_t>::difference_type
		{ return lhs.base() - rhs.base(); }

	template<typename iterator_t, typename container_t> inline auto operator +(
		const typename normalIterator_t<iterator_t, container_t>::difference_type n,
		const normalIterator_t<iterator_t, container_t> &iter
	) noexcept -> normalIterator_t<iterator_t, container_t>
		{ return normalIterator_t<iterator_t, container_t>{iter.base() + n}; }
} // namespace flashprog::utils

#endif /*UTILS_ITERATOR__HXX*/
