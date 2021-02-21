#include <cstring>

#include "tokenizer.hxx"

namespace flashprog::args::tokenizer
{
	void tokenizer_t::nextArg() noexcept
	{
		if (currentArg == count)
		{
			arg = nullptr;
			offset = nullptr;
			length = 0;
		}
		else
		{
			arg = args[currentArg++];
			offset = arg;
			length = strlen(arg);
		}
	}

	[[nodiscard]] bool isEquals(const char c) noexcept { return c == '='; }

	void tokenizer_t::readToken() noexcept
	{
		if (!arg)
			token_ = {};
		else if (offset == arg + length)
		{
			nextArg();
			if (!arg)
				token_ = {};
			else
				token_ = {tokenType_t::space};
		}
		else if (isEquals(*offset))
		{
			++offset;
			token_ = {tokenType_t::equals, {"=", 1}};
		}
		else
		{
			const auto *const begin = offset;
			while (offset < arg + length)
			{
				if (isEquals(*offset))
					break;
				++offset;
			}
			token_ = {tokenType_t::arg, {begin, size_t(offset - begin)}};
		}
	}

	const char *typeToName(const tokenType_t type)
	{
		switch (type)
		{
		case tokenType_t::unknown:
			return "unknown";
		case tokenType_t::arg:
			return "argument";
		case tokenType_t::space:
			return "implicit space";
		case tokenType_t::equals:
			return "equals sign";
		}
		return nullptr;
	}
} // namespace flashprog::args::tokenizer
