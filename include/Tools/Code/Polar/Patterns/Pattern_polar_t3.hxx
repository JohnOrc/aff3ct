#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r1.hpp"
#include "Tools/Code/Polar/Patterns/Pattern_polar_r0.hpp"

namespace aff3ct
{
namespace tools
{
Pattern_polar_t3
::Pattern_polar_t3(const int &N, const Binary_node<Pattern_polar_i>* node, const int min_level, const int max_level)
: Pattern_polar_i(N, node, min_level, max_level)
{
	if (min_level < 1)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_t3
::Pattern_polar_t3(const int min_level, const int max_level)
: Pattern_polar_i(min_level, max_level)
{
	if (min_level < 1)
	{
		std::stringstream message;
		message << "'min_level' has to be equal or greater than 1 ('min_level' = " << min_level << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

Pattern_polar_t3* Pattern_polar_t3
::alloc(const int &N, const Binary_node<Pattern_polar_i>* node) const
{
	if (node == nullptr)
		return new Pattern_polar_t3(min_level, max_level);
	else
		return new Pattern_polar_t3(N, node, min_level, max_level);
}

polar_node_t Pattern_polar_t3
::type()       const
{
	// todo 
	return polar_node_t::T3;
}

std::string  Pattern_polar_t3
::name()       const
{
	return "T3";
}

std::string  Pattern_polar_t3
::short_name() const
{
	return "3";
}

std::string  Pattern_polar_t3
::fill_color() const
{
	return "#2F3FFF";
}

std::string  Pattern_polar_t3
::font_color() const
{
	return "#FFFFFF";
}

std::string Pattern_polar_t3
::f() const
{
	return "";
}

std::string Pattern_polar_t3
::g() const
{
	return "";
}

std::string Pattern_polar_t3
::h() const
{
	return "t3";
}

bool Pattern_polar_t3
::recursive_check(const int reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr)
{
	if (!node_curr->is_leaf())
	{
		const auto pattern_left  = node_curr->get_left ()->get_contents();
		const auto pattern_right = node_curr->get_right()->get_contents();

		if (pattern_right->type() == polar_node_t::RATE_1 ||
		    Pattern_polar_r1::recursive_check(reverse_graph_depth -1, node_curr->get_right()))
		{
			if (pattern_left->type() == polar_node_t::T3)
				return true;
			else if (pattern_left->type() == polar_node_t::RATE_0 && reverse_graph_depth == 2)
				return true;
			else
				return Pattern_polar_t3::recursive_check(reverse_graph_depth -1, node_curr->get_left());
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int Pattern_polar_t3
::_match(const int &reverse_graph_depth, const Binary_node<Pattern_polar_i>* node_curr) const
{
	// todo 49 ?
	return Pattern_polar_t3::recursive_check(reverse_graph_depth, node_curr) ? 49 : 0;
}

bool Pattern_polar_t3
::is_terminal() const
{
	return true;
}
}
}
