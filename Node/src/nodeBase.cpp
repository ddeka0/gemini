#include "nodeBase.hpp"
#include "Chord/include/utils.hpp" // for hashing

NodeBase::NodeBase(){
	PRINT_FUNC_NAME;
}
NodeBase::~NodeBase(){
	PRINT_FUNC_NAME;
}

unsigned int NodeBase::getId(unsigned int x) {
	PRINT_FUNC_NAME;
	unsigned int res = sha256(m_addr->toString());
	std::cout <<"res = "<<res<<std::endl;
	return res;
}

void NodeBase::Get() {
	// Debug Print
	std::cout << "Get() called with a NodeBase object" << std::endl;
}

void NodeBase::initWithServer(std::shared_ptr<GrpcAsyncServer>) {
	std::cout<<"NodeBase initWithServer() called" << std::endl;
}
void NodeBase::initWithClient(std::shared_ptr<GrpcAsyncClient>) {
	std::cout<<"NodeBase initWithClient() called" << std::endl;
}