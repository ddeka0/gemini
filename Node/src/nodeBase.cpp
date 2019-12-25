#include "nodeBase.hpp"

NodeBase::NodeBase(){}
NodeBase::~NodeBase(){}

unsigned int NodeBase::getId(unsigned int x) {
	// TODO implement consistent hash of IP+PORT
	// return an int value
}

void NodeBase::Get() {
	// Debug Print
	std::cout << "Get() called with a NodeBase object" << std::endl;
}