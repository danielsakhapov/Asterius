#include "SyntaxStructures.h"

namespace asterius
{

TransitionRule::TransitionRule(ElementType&& term, std::vector<ElementType>&& vec) :
	term_(std::move(term)), vec_(std::move(vec))
{}

}