#include "RPNStructures.h"

namespace asterius
{

Variable::Variable(DataType type, void* data) :
	type_(type), data_(data)
{}

}