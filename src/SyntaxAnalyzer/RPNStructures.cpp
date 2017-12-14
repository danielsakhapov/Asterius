#include "RPNStructures.h"

namespace asterius
{

Variable::Variable(const DataType& type) :
	type_(type), data_(nullptr)
{}

Variable::Variable(const DataType& type, void* data) :
	type_(type), data_(data)
{}

Command::Command(void* data) :
	data_(data)
{}

IntCommand::IntCommand(void* data) :
	Command(data)
{}

DoubleCommand::DoubleCommand(void* data) :
	Command(data)
{}

ByteCommand::ByteCommand(void* data) :
	Command(data)
{}

StringCommand::StringCommand(void* data) :
	Command(data)
{}

IntConstCommand::IntConstCommand(void* data) :
	Command(data)
{}

DoubleConstCommand::DoubleConstCommand(void* data) :
	Command(data)
{}

ByteConstCommand::ByteConstCommand(void* data) :
	Command(data)
{}

StringConstCommand::StringConstCommand(void* data) :
	Command(data)
{}

ZeroConstCommand::ZeroConstCommand(void* data) :
	Command(data)
{}

NameCommand::NameCommand(void* data) :
	Command(data)
{}

}