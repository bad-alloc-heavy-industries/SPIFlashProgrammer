#ifndef ARGS__HXX
#define ARGS__HXX

#include <vector>
#include <array>
#include <memory>
#include <string_view>
#include <limits>
#include "utils/span.hxx"

using namespace std::literals::string_view_literals;

namespace flashprog::args
{
	enum class argType_t : uint8_t
	{
		tree,
		unrecognised,
		help,
		version,
		listDevices,
		list,
		read,
		write,
		verifiedWrite,
		device,
		chip,
		file
	};

	enum class ensure_t
	{
		none,
		one,
		many
	};

	struct argNode_t
	{
	private:
		argType_t type_;

	public:
		constexpr argNode_t(argType_t type) noexcept : type_{type} { }
		argNode_t(const argNode_t &) = delete;
		argNode_t(argNode_t &&) = delete;
		virtual ~argNode_t() noexcept = default;
		argNode_t &operator =(const argNode_t &) = delete;
		argNode_t &operator =(argNode_t &&) = delete;
		[[nodiscard]] constexpr auto type() const noexcept { return type_; }
	};

	struct argsTree_t : argNode_t
	{
	private:
		std::vector<std::unique_ptr<argNode_t>> children_;

	protected:
		argsTree_t(argType_t type) noexcept : argNode_t{type}, children_{} { }

	public:
		argsTree_t() noexcept : argNode_t{argType_t::tree}, children_{} { }
		[[nodiscard]] argNode_t *find(argType_t type) const noexcept;
		[[nodiscard]] bool add(std::unique_ptr<argNode_t> &&node) noexcept;

		template<typename... types_t>
			[[nodiscard]] argNode_t *findAny(argType_t type, types_t ...types) const noexcept
		{
			if (auto *const result{find(type)}; result)
				return result;
			if constexpr (sizeof...(types))
				return findAny(types...);
			else
				return nullptr;
		}

		template<typename... types_t>
			[[nodiscard]] ensure_t maybeEnsureOneOf(argType_t type, types_t ...types) const noexcept
		{
			if constexpr (sizeof...(types))
			{
				if (find(type))
					return findAny(types...) ? ensure_t::many : ensure_t::one;
			}
			else
			{
				if (find(type))
					return ensure_t::one;
			}
			return ensure_t::none;
		}

		[[nodiscard]] auto count() const noexcept { return children_.size(); }
		[[nodiscard]] auto size() const noexcept { return children_.size(); }

		[[nodiscard]] auto begin() const noexcept { return children_.begin(); }
		[[nodiscard]] auto end() const noexcept { return children_.end(); }
	};

	struct argUnrecognised_t final : argNode_t
	{
	private:
		std::string_view argument_{};
		std::string_view parameter_{};

	public:
		argUnrecognised_t() = delete;
		constexpr argUnrecognised_t(const std::string_view argument) noexcept :
			argNode_t{argType_t::unrecognised}, argument_{argument}, parameter_{} { }
		constexpr argUnrecognised_t(const std::string_view argument, const std::string_view parameter) noexcept :
			argNode_t{argType_t::unrecognised}, argument_{argument}, parameter_{parameter} { }
		[[nodiscard]] constexpr auto argument() const noexcept { return argument_; }
		[[nodiscard]] constexpr auto parameter() const noexcept { return parameter_; }
	};

	struct argList_t final : argsTree_t
	{
	public:
		argList_t() noexcept : argsTree_t{argType_t::list} { }
		constexpr static auto name() noexcept { return "list"sv; }
	};

	struct argRead_t final : argsTree_t
	{
	public:
		argRead_t() noexcept : argsTree_t{argType_t::read} { }
		constexpr static auto name() noexcept { return "read"sv; }
	};

	struct argWrite_t final : argsTree_t
	{
	public:
		argWrite_t() noexcept : argsTree_t{argType_t::write} { }
		constexpr static auto name() noexcept { return "write"sv; }
	};

	struct argVerifiedWrite_t final : argsTree_t
	{
	public:
		argVerifiedWrite_t() noexcept : argsTree_t{argType_t::verifiedWrite} { }
		constexpr static auto name() noexcept { return "verifiedWrite"sv; }
	};

	struct argDevice_t final : argNode_t
	{
	private:
		uint16_t deviceNumber_{};

	public:
		argDevice_t(const std::string_view device) noexcept;
		[[nodiscard]] auto valid() const noexcept { return deviceNumber_ != invalidDevice; }
		[[nodiscard]] auto deviceNumber() const noexcept { return deviceNumber_; }

		constexpr static auto invalidDevice = std::numeric_limits<uint16_t>::max();
	};

	template<argType_t argType> struct argOfType_t final : argNode_t
	{
	public:
		constexpr argOfType_t() noexcept : argNode_t{argType} { }
	};

	using argHelp_t = argOfType_t<argType_t::help>;
	using argVersion_t = argOfType_t<argType_t::version>;
	using argListDevices_t = argOfType_t<argType_t::listDevices>;

	struct option_t final
	{
	private:
		std::string_view option_;
		argType_t type_;

	public:
		constexpr option_t(const std::string_view option, const argType_t type) noexcept :
			option_{option}, type_{type} { }

		[[nodiscard]] constexpr auto name() const noexcept { return option_; }
		[[nodiscard]] constexpr auto option() const noexcept { return option_; }
		[[nodiscard]] constexpr auto type() const noexcept { return type_; }
	};
} // namespace flashprog::args

using flashprog::args::argType_t;

// parseArguments() as a free function that takes argc, argv and
// a descriptor structure containing information on the supported arguments as a
// parsing tree, and returns a new argsTree_t std::unique_ptr<> or nullptr

// The goal of this subsystem is to make further argument handling cheap by not having
// to continuously re-run strcmp(), having done that the minimum number of times possible
// in the args parser and encoded as much of the results as simple integers as reasonable

extern std::unique_ptr<flashprog::args::argsTree_t> args;
using flashprog::args::argType_t;

extern bool parseArguments(size_t argCount, const char *const *argList,
	flashprog::utils::span_t<const flashprog::args::option_t> options);

#endif /*ARGS__HXX*/
