#include <substrate/console>
#include <substrate/utility>
#include "args.hxx"
#include "args/tokenizer.hxx"

using substrate::console;
using flashprog::utils::span_t;
using namespace flashprog::args;
using namespace flashprog::args::tokenizer;
using namespace std::literals::string_view_literals;

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<argsTree_t> args{};

template<typename node_t> auto parsePerDeviceCommand(tokenizer_t &lexer)
{
	auto node{substrate::make_unique<node_t>()};
	const auto &token{lexer.token()};
	if (token.type() == tokenType_t::unknown)
	{
		console.error(node_t::name(), " command was given but no SPI Flash chip number was specified"sv);
		throw std::exception{};
	}
	lexer.next();
	if (token.value() == "--device"sv)
	{
		lexer.next();
		lexer.next();
		auto device{substrate::make_unique<argDevice_t>(token.value())};
		if (!device->valid())
		{
			console.error("Device number must be given as a positive integer between 0 and 65534"sv);
			throw std::exception{};
		}
		lexer.next();
		lexer.next();
		if (!node->add(std::move(device)))
		{
			console.error("Could not add argument to the parsed arguments tree"sv);
			throw std::exception{};
		}
	}
	return node;
}

template<typename node_t> auto parsePerFlashCommand(tokenizer_t &lexer)
{
	auto node{parsePerDeviceCommand<node_t>(lexer)};
	const auto &token{lexer.token()};
	if (token.type() == tokenType_t::unknown)
	{
		console.error(node_t::name(), " command was given but no SPI Flash chip number was specified"sv);
		throw std::exception{};
	}
	auto flashChip{substrate::make_unique<argChip_t>(token.value())};
	if (!flashChip->valid())
	{
		console.error("Chip number for operation must be given as a positive integer between 0 and 65535"sv);
		throw std::exception{};
	}
	lexer.next();
	if (!node->add(std::move(flashChip)))
	{
		console.error("Could not add argument to the parsed arguments tree"sv);
		throw std::exception{};
	}
	if constexpr (!std::is_same_v<node_t, argErase_t>)
	{
		lexer.next();
		auto file{substrate::make_unique<argFile_t>(token.value())};
		if (!file->valid())
		{
			console.error("File name must be a valid path to a file to read/write"sv);
			throw std::exception{};
		}
		lexer.next();
		if (!node->add(std::move(file)))
		{
			console.error("Could not add argument to the parsed arguments tree"sv);
			throw std::exception{};
		}
	}
	return node;
}

std::unique_ptr<argNode_t> makeNode(tokenizer_t &lexer, const option_t &option)
{
	lexer.next();
	switch (option.type())
	{
		case argType_t::help:
			return substrate::make_unique<argHelp_t>();
		case argType_t::version:
			return substrate::make_unique<argVersion_t>();
		case argType_t::listDevices:
			return substrate::make_unique<argListDevices_t>();
		case argType_t::list:
			return parsePerDeviceCommand<argList_t>(lexer);
		case argType_t::erase:
			return parsePerFlashCommand<argErase_t>(lexer);
		case argType_t::read:
			return parsePerFlashCommand<argRead_t>(lexer);
		case argType_t::write:
			return parsePerFlashCommand<argWrite_t>(lexer);
		case argType_t::verifiedWrite:
			return parsePerFlashCommand<argVerifiedWrite_t>(lexer);
		default:
			throw std::exception{};
	}
}

bool parseArgument(tokenizer_t &lexer, const span_t<const option_t> &options, argsTree_t &ast)
{
	const auto &token{lexer.token()};
	if (token.type() == tokenType_t::space)
		lexer.next();
	else if (token.type() != tokenType_t::arg)
		return false;
	const auto argument{token.value()};
	for (const auto &option : options)
	{
		if (argument == option.name())
			return ast.add(makeNode(lexer, option));
	}
	lexer.next();
	if (token.type() != tokenType_t::equals)
	{
		if (!ast.add(substrate::make_unique<argUnrecognised_t>(argument)))
			return false;
	}
	else
	{
		lexer.next();
		if (token.type() == tokenType_t::space)
		{
			if (!ast.add(substrate::make_unique<argUnrecognised_t>(argument)))
				return false;
		}
		else
		{
			if (!ast.add(substrate::make_unique<argUnrecognised_t>(argument, token.value())))
				return false;
			lexer.next();
		}
	}
	return true;
}

// Recursive descent parser that efficiently matches the current token from argv against
// the set of allowed arguments at the current parsing level, and returns their AST
// representation if matched, allowing the parser to build a neat tree of all
// the arguments for further use by the caller
bool parseArguments(const size_t argCount, const char *const *const argList,
	const span_t<const option_t> options) try
{
	if (argCount < 1 || !argList)
		return false;
	// Skip the first argument (that's the name of the program) and start
	// tokenizing directly at the second.
	tokenizer_t lexer{argCount - 1, argList + 1};
	const auto &token{lexer.token()};
	args = substrate::make_unique<argsTree_t>();

	while (token.valid())
	{
		if (!parseArgument(lexer, options, *args))
		{
			std::string argument{token.value()};
			console.warn("Found invalid token '"sv, argument, "' ("sv, // NOLINT(readability-magic-numbers)
				typeToName(token.type()), ") in arguments"sv); // NOLINT(readability-magic-numbers)
			return false;
		}
	}
	return true;
}
catch (std::exception &)
	{ return false; }
