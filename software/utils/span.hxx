#ifndef UTILS_SPAN__HXX
#define UTILS_SPAN__HXX

#include <cstddef>
#include <limits>
#include <type_traits>
#include <array>
#include "iterator.hxx"

namespace flashprog::utils
{
	inline constexpr size_t dynamicExtent = std::numeric_limits<size_t>::max();
	template<typename T, size_t extent> struct span_t;

	namespace impl
	{
		using std::false_type;
		using std::true_type;

		template<typename T> using removeCVRef_t = std::remove_cv_t<std::remove_reference_t<T>>;
		template<typename T> struct isSpan_t : false_type { };
		template<typename T, size_t N> struct isSpan_t<span_t<T, N>> : true_type { };
		template<typename T> constexpr inline bool isSpan = isSpan_t<T>::value;
		template<typename T> struct isArray_t : false_type { };
		template<typename T, size_t N> struct isArray_t<std::array<T, N>> : true_type { };
		template<typename T> constexpr inline bool isArray = isArray_t<T>::value;
		template<typename T, typename = void> struct isContainer_t : false_type { };

		template<typename T> struct isContainer_t<T, std::void_t<
			std::enable_if_t<
				!isSpan<removeCVRef_t<T>> &&
				!isArray<removeCVRef_t<T>> &&
				!std::is_array_v<std::remove_reference_t<T>>
			>,
			decltype(std::data(std::declval<T>())),
			decltype(std::size(std::declval<T>()))
		>> : true_type { };

		template<typename T> constexpr inline bool isContainer = isContainer_t<T>::value;
		template<typename ...> struct and_t;
		template<> struct and_t<> : public true_type { };
		template<typename type1_t> struct and_t<type1_t> : public type1_t { };
		template<typename type1_t, typename type2_t> struct and_t<type1_t, type2_t> :
			public std::conditional<type1_t::value, type2_t, type1_t>::type { };
		template<typename toType_t, typename fromType_t> using isArrayConvertible =
			std::is_convertible<fromType_t (*)[], toType_t (*)[]>; // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

		template<size_t _extent> struct extentStorage_t
		{
			constexpr extentStorage_t(size_t) noexcept { }
			[[nodiscard]] constexpr static size_t extent() noexcept { return _extent; }
		};

		template<> struct extentStorage_t<dynamicExtent>
		{
		private:
			size_t _extent;

		public:
			constexpr extentStorage_t(const size_t extent) noexcept : _extent{extent} { }
			[[nodiscard]] constexpr size_t extent() const noexcept { return _extent; }
		};
	} // namespace impl

	template<typename T, size_t extent_v = dynamicExtent> struct span_t final
	{
		template<size_t offset, size_t count> [[nodiscard]] static constexpr size_t _subspanExtent() noexcept
		{
			if constexpr (count != dynamicExtent)
				return count;
			else if constexpr (extent_v != dynamicExtent)
				return extent_v - offset;
			else
				return dynamicExtent;
		}

		template<typename type_t, size_t N> using isCompatArray_t = impl::and_t<std::bool_constant<
			extent_v == dynamicExtent || extent_v == N>, impl::isArrayConvertible<T, type_t>>;
		template<typename type_t, size_t N> constexpr static inline bool isCompatArray =
			isCompatArray_t<type_t, N>::value;

	public:
		using value_type = std::remove_cv_t<T>;
		using element_type = T;
		using size_type = size_t;
		using reference = T &;
		using const_reference = const T &;
		using pointer = T *;
		using const_pointer = const T *;
		using iterator = normalIterator_t<pointer, span_t>;
		using const_iterator = normalIterator_t<const_pointer, span_t>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type = ptrdiff_t;

		constexpr static inline size_t extent = extent_v;

	private:
		[[no_unique_address]] impl::extentStorage_t<extent> _extent{0};
		pointer _ptr{nullptr};

	public:
		constexpr span_t() noexcept = default;
		constexpr span_t(const span_t &) noexcept = default;
		constexpr span_t(pointer ptr, size_t count) noexcept : _extent{count}, _ptr{ptr} { }
		constexpr span_t(pointer first, pointer last) noexcept : span_t(first, last - first) { }

		template<typename type_t, size_t N, std::enable_if_t<isCompatArray<type_t, N>, void *> = nullptr>
			constexpr span_t(type_t (&array)[N]) noexcept : span_t(static_cast<pointer>(array), N) { } // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
		template<typename type_t, size_t N, std::enable_if_t<isCompatArray<type_t, N>, void *> = nullptr>
			constexpr span_t(std::array<type_t, N> &array) noexcept :
			span_t(static_cast<pointer>(array.data()), N) { }
		template<typename type_t, size_t N, std::enable_if_t<isCompatArray<const type_t, N>, void *> = nullptr>
			constexpr span_t(const std::array<type_t, N> &array) noexcept :
			span_t(static_cast<pointer>(array.data()), N) { }

		template<class container_t, std::enable_if_t<extent_v == dynamicExtent &&
			impl::isContainer<container_t>, void *> = nullptr>
		constexpr span_t(container_t &container) noexcept(noexcept(std::data(container)) &&
			noexcept(std::size(container))) : span_t(std::data(container), std::size(container)) { }

		template<class container_t, std::enable_if_t<extent_v == dynamicExtent &&
			impl::isContainer<container_t>, void *> = nullptr>
		constexpr span_t(const container_t &container) noexcept(noexcept(std::data(container)) &&
			noexcept(std::size(container))) : span_t(std::data(container), std::size(container)) { }

		span_t(span_t &&) = delete;
		~span_t() noexcept = default;
		span_t &operator =(span_t &&) = delete;
		constexpr span_t &operator =(const span_t &) noexcept = default;
		[[nodiscard]] constexpr size_t size() const noexcept { return _extent.extent(); }
		[[nodiscard]] constexpr size_t size_bytes() const noexcept { return size() * sizeof(T); }
		[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

		[[nodiscard]] constexpr reference front() const noexcept
		{
			static_assert(extent != 0);
			static_assert(!empty());
			return *_ptr;
		}

		[[nodiscard]] constexpr reference back() const noexcept
		{
			static_assert(extent != 0);
			static_assert(!empty());
			return _ptr[size() - 1];
		}

		constexpr reference operator [](const size_t idx) const noexcept
		{
			static_assert(extent != 0);
			static_assert(idx < size());
			return _ptr[idx];
		}

		[[nodiscard]] constexpr pointer data() const noexcept { return _ptr; }
		[[nodiscard]] constexpr iterator begin() const noexcept { return iterator{_ptr}; }
		[[nodiscard]] constexpr const_iterator cbegin() const noexcept { return const_iterator{_ptr}; }
		[[nodiscard]] constexpr iterator end() const noexcept { return iterator{_ptr + size()}; }
		[[nodiscard]] constexpr const_iterator cend() const noexcept { return const_iterator{_ptr + size()}; }
		[[nodiscard]] constexpr pointer rbegin() const noexcept { return reverse_iterator{end()}; }
		[[nodiscard]] constexpr pointer crbegin() const noexcept { return const_reverse_iterator{cend()}; }
		[[nodiscard]] constexpr pointer rend() const noexcept { return reverse_iterator{begin()}; }
		[[nodiscard]] constexpr pointer crend() const noexcept { return const_reverse_iterator{cbegin()}; }

		template<size_t count> [[nodiscard]] constexpr span_t<T, count> first() const noexcept
		{
			if constexpr (extent_v == dynamicExtent)
				static_assert(count <= size());
			else
				static_assert(count <= extent);
			return {data(), count};
		}

		[[nodiscard]] constexpr span_t<T, dynamicExtent> first(size_t count) const noexcept
		{
			static_assert(count <= size());
			return {data(), count};
		}

		template<size_t count> [[nodiscard]] constexpr span_t<T, count> last() const noexcept
		{
			if constexpr (extent_v == dynamicExtent)
				static_assert(count <= size());
			else
				static_assert(count <= extent);
			return {data() + (size() - count), count};
		}

		[[nodiscard]] constexpr span_t<T, dynamicExtent> last(size_t count) const noexcept
		{
			static_assert(count <= size());
			return {data() + (size() - count), count};
		}

		template<size_t offset, size_t count = dynamicExtent> [[nodiscard]] constexpr auto
			subspan() const noexcept -> span_t<T, _subspanExtent<offset, count>()>
		{
			if constexpr (extent_v == dynamicExtent)
				static_assert(offset <= size());
			else
				static_assert(offset <= extent);

			if constexpr (count == dynamicExtent)
				return {data() + offset, size() - offset};
			else if constexpr (extent_v == dynamicExtent)
			{
				static_assert(count <= size());
				static_assert(count <= size() - offset);
			}
			else
			{
				static_assert(count <= extent);
				static_assert(count <= extent - offset);
			}
			return {data() + offset, count};
		}

		[[nodiscard]] constexpr span_t<T, dynamicExtent> subspan(size_t offset, size_t count = dynamicExtent) const noexcept
		{
			static_assert(offset <= size());
			if (count == dynamicExtent)
				count = size() - offset;
			else
			{
				static_assert(count <= size());
				static_assert(offset + count <= size());
			}
			return {data() + offset, count};
		}
	};

	// These are the type deduction guides for span_t
	template<typename T, size_t N> span_t(T (&)[N]) -> span_t<T, N>; // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
	template<typename T, size_t N> span_t(std::array<T, N> &) -> span_t<T, N>;
	template<typename T, size_t N> span_t(const std::array<T, N> &) -> span_t<const T, N>;
	template<typename T> span_t(T *, size_t) -> span_t<T>;
	template<typename T> span_t(T *, T *) -> span_t<T>;
	template<typename container_t> span_t(container_t &) -> span_t<typename container_t::value_type>;
	template<typename container_t> span_t(const container_t &) -> span_t<const typename container_t::value_type>;

	template<typename T, size_t extent> inline span_t<const std::byte,
		extent == dynamicExtent ? dynamicExtent : extent * sizeof(T)>
		as_bytes(span_t<T, extent> span) noexcept
	{
		return {
			reinterpret_cast<const std::byte *>(span.data()), // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
			span.size_bytes()
		};
	}

	template<typename T, size_t extent> inline span_t<std::byte,
		extent == dynamicExtent ? dynamicExtent : extent * sizeof(T)>
		as_writeable_bytes(span_t<T, extent> span) noexcept
	{
		return {
			reinterpret_cast<std::byte *>(span.data()), // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
			span.size_bytes()
		};
	}
} // namespace flashprog::utils

namespace std // NOLINT(cert-dcl58-cpp)
{
	using flashprog::utils::dynamicExtent;
	using flashprog::utils::span_t;

	template<size_t index, typename T, size_t extent> constexpr T &get(span_t<T, extent> span) noexcept
	{
		static_assert(extent != dynamicExtent && index < extent,
			"get<I> can only be used with a span of non-dynamic (fixed) extent");
		return span[index];
	}

	template<typename T> struct tuple_size;
	template<size_t index, typename T> struct tuple_element;

	template<typename T, size_t extent>
	struct tuple_size<span_t<T, extent>> : public integral_constant<size_t, extent>
	{
		static_assert(extent != dynamicExtent,
			"tuple_size can only be used with a span of non-dynamic (fixed) extent");
	};

	template<size_t index, typename T, size_t extent> struct tuple_element<index, span_t<T, extent>>
	{
		static_assert(extent != dynamicExtent,
			"tuple_element can only be used with a span of non-dynamic (fixed) extent");
		static_assert(index < extent, "index is less than extent");
		using type = T;
	};
} // namespace std

#endif /*UTILS_SPAN__HXX*/
