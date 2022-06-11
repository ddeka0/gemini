#include "serviceBase.hpp"

ServiceBase::ServiceBase(){
	// Debug print
	PRINT_FUNC_NAME;
};

ServiceBase::~ServiceBase(){
	// Debug print
	PRINT_FUNC_NAME;
}

void ServiceBase::initialize(NodeBase* node) {
	// set the parent node pointer first
	PRINT_FUNC_NAME;
	if(nullptr != node) {
		m_node = node;
	}else {
		// TODO use log system
	}
	// m_table (on the left hand side is a pointer, just to store the actual
	// instance of m_able which is owned by the Node but not by the ServiceBase) 
	m_table = m_node->m_table.get();

}

void ServiceBase::Get() {
	// Debug print
	std::cout << "Get() called from ServiceBase" << std::endl;
}

