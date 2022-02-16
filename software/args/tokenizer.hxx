#ifndef ARGS_TOKENIZER_HXX
#define ARGS_TOKENIZER_HXX

#include <cstddef>
#include <string_view>

namespace flashprog::args::tokenizer
{
	enum class tokenType_t : uint8_t
	{
		unknown,
		arg,
		space,
		equals
	};

	struct token_t final
	{
	private:
		tokenType_t type_{tokenType_t::unknown};
		std::string_view value_{};

	public:
		constexpr token_t() noexcept = default;
		constexpr token_t(const tokenType_t type) noexcept : type_{type}, value_{} { }
		token_t(const tokenType_t type, std::string_view &&value) noexcept :
			type_{type}, value_{value} { }

		[[nodiscard]] bool valid() const noexcept { return type_ != tokenType_t::unknown; }
		[[nodiscard]] tokenType_t type() const noexcept { return type_; }
		[[nodiscard]] std::string_view value() const noexcept { return value_; }
	};

	struct tokenizer_t final
	{
	private:
		size_t currentArg;
		const char *arg;
		const char *offset;
		size_t length;
		token_t token_;
		size_t count;
		const char *const *args;

		void nextArg() noexcept;
		void readToken() noexcept;

	public:
		tokenizer_t(const size_t argsCount, const char *const *const argsList) noexcept :
			currentArg{}, arg{}, offset{}, length{}, token_{}, count{argsCount}, args{argsList}
			{ nextArg(); next(); }

		[[nodiscard]] const token_t &token() const noexcept { return token_; }
		token_t &token() noexcept { return token_; }

		token_t &next() noexcept
		{
			readToken();
			return token_;
		}
	};

	const char *typeToName(tokenType_t type);
} // namespace flashprog::args::tokenizer

#endif /*ARGS_TOKENIZER_HXX*/
