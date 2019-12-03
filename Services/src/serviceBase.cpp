#include "serviceBase.hpp"

ServiceBase::ServiceBase(){};

ServiceBase::~ServiceBase(){};

void ServiceBase::initialize(NodeBase* node) {
	if(nullptr != node) {
		m_node = node;
	}else {
		// TODO use log system
	}
}
NodeBase* ServiceBase::findSuccessor() {
	return m_table->Map[1]; 
}
void ServiceBase::fixFingers() {
	// TODO need to chamnge stopping mechanism of this while loop
	bool running = true;
	int nxt = 0;
	while(running) {
		nxt++;
		// TODO place NBITS in proper location
		if(nxt > NBITS) {
			nxt = 1;
		}
		// TODO write operator overload for [] op
		// overload the [] operator to mask the Map object
		// m_table->Map[nxt - 1] = findSuccessor();
		((*m_table)[nxt - 1]) = findSuccessor();
		// m_table[nxt - 1] = 
		// TODO make inclide.hpp header file for all such such function
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

bool inrange(unsigned int a,unsigned int b,unsigned int c) {
	// TODO implement inrange in proper directory and file
	return false;
}

NodeBase* ServiceBase::closestPrecedingNode(unsigned int Id) {
	for(int idx = NBITS - 1;idx >=0 ;idx--) {
		if(m_table->Map[idx]->getId() != -1 and
			inrange(m_table->Map[idx]->getId(),m_node->getId(),Id) and
			(m_table->Map[idx]->getId() != m_node->getId()) and 
			Id != m_table->Map[idx]->getId()) 
		{
			return m_table->Map[idx];
		} 
	}
	return m_node; // try to use a good macro for this like SELF, THIS_NODE etc.
}
