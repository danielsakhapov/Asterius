#include "SyntaxStructures.h"

namespace asterius
{

TransitionRule::TransitionRule(std::vector<ElementType>&& elements, std::vector<ActionType>&& acts) :
	elements_(std::move(elements)), acts_(std::move(acts))
{}

}