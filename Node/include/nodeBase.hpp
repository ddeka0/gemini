#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/address.hpp"
/*
This is a parent node class.
It should have:
1. Address
2. 
*/
class NodeBase {
public:
	NodeBase(){};
	~NodeBase(){};
	NodeBase(const Address & addr);
	void initilize();
	Address * getAddress();
	void setAddress();
	unsigned int getId();
protected:
private:
	Address m_addr;
};