#include "nodeBase.hpp"
void NodeBase::initilize(){}
NodeBase::NodeBase(const Address& addr) {
	m_addr = addr;
}
Address * NodeBase::getAddress() {
	return &m_addr;
}