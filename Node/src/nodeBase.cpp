#include "nodeBase.hpp"
void NodeBase::initilize(){}
NodeBase::NodeBase(const Address& addr) {
	m_addr = addr;
}
Address * NodeBase::getAddress() {
	return &m_addr;
}

unsigned int NodeBase::getId() {
	// TODO implement consistent hash of IP+PORT
	// return an int value
}