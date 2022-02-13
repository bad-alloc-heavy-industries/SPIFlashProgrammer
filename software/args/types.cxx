#include <substrate/conversions>
#include <substrate/console>
#include "../args.hxx"

using namespace std::literals::string_view_literals;
using substrate::toInt_t;
using substrate::console;

namespace flashprog::args
{
	argNode_t *argsTree_t::find(const argType_t nodeType) const noexcept
	{
		for (const auto &node : children_)
		{
			if (node->type() == nodeType)
				return node.get();
		}
		return nullptr;
	}

	bool argsTree_t::add(std::unique_ptr<argNode_t> &&node) noexcept try
		{ return bool{children_.emplace_back(std::move(node))}; }
	catch (std::bad_alloc &)
		{ return false; }

	argDevice_t::argDevice_t(const std::string_view device) noexcept :
		argNode_t{argType_t::device}, deviceNumber_{invalidDevice}
	{
		toInt_t<uint16_t> number{device.data(), device.size()};
		if (number.isDec())
			deviceNumber_ = number.fromDec();
	}

	argChip_t::argChip_t(const std::string_view device) :
		argNode_t{argType_t::chip}, chipNumber_{invalidChip}
	{
		const auto colon{device.find(':')};
		if (colon == std::string_view::npos)
		{
			toInt_t<uint16_t> number{device.data(), device.size()};
			if (number.isDec())
				console.error("Expected a bus specification prior to chip number"sv);
			else
				console.error("Expected a bus specification, got '"sv, device, '\'');
			throw std::exception{};
		}
		const auto bus{device.substr(0, colon)};
		const auto chip{device.substr(colon + 1)};

		if (bus == "int")
			bus_ = flashBus_t::internal;
		else if (bus == "ext")
			bus_ = flashBus_t::external;
		else
		{
			console.error("Expected one of 'int' or 'ext' for bus specification, got '"sv, bus, '\'');
			throw std::exception{};
		}

		toInt_t<uint16_t> chipNumber{chip.data(), chip.size()};
		if (chipNumber.isDec())
			chipNumber_ = chipNumber.fromDec();
	}
} // namespace flashprog::args
