#include <substrate/conversions>
#include "../args.hxx"

using namespace std::literals::string_view_literals;
using substrate::toInt_t;

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
} // namespace flashprog::args
