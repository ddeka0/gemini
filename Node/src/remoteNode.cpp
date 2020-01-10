#include "remoteNode.hpp"
#include "Chord/include/utils.hpp" // for hashing

/* Constructor of LocalNode class
*/
RemoteNode::RemoteNode() {
    std::cout << this <<" RemoteNode instance created" << std::endl;
}

void RemoteNode::join(Address * remote_addr) {
    std::cout <<"RemoteNode join(), Invalid call" << std::endl;
}
/* Destructor of LocalNode class
*/
RemoteNode::~RemoteNode() {
    // Debug print
    std::cout << this <<" RemoteNode instance destroyed" << std::endl;
};

void RemoteNode::initWithClient(std::shared_ptr<GrpcAsyncClient> _client) {
    std::cout << "RemoteNode Initialized"<< std::endl;
    m_client = _client;
}

/*
This function assigns address to the node
*/
void RemoteNode::setAddress(std::shared_ptr<Address> _addr) {
    m_addr = _addr;
}

std::shared_ptr<Address> RemoteNode::getAddress() {
    return m_addr;
}
/*
This function takes argument as bitmask
for example GET | PUT | DEL etc.
and the implementation perform service adding or deletion for this node
*/
void RemoteNode::addServices(uint8_t flag) {
    std::cout <<"RemoteNode addServices(), invalid call" << std::endl;
}

void RemoteNode::Start() {
    std::cout <<"RemoteNode Start(), invalid call" << std::endl;
}

void RemoteNode::Get() {
    std::cout <<"RemoteNode Get(), invalid call" << std::endl;
}

NodeBase* RemoteNode::getPredecessor() {

    return m_client->getPredecessor();
}

NodeBase* RemoteNode::getSuccessor() {
    // TODO Setup RPC call
}

void RemoteNode::setSuccessor(NodeBase * node) {
    std::cout <<"RemoteNode setSuccessor(),invalid call" << std::endl;
}
void RemoteNode::setPredecessor(NodeBase * node) {
    std::cout <<"RemoteNode setPredecessor(),invalid call" << std::endl;
}

unsigned int RemoteNode::getId(unsigned int x) {
	// std::cout<<__func__<<std::endl;
	unsigned int res = sha256(m_addr->toString()) + x;
	// std::cout <<"res = "<<res<<std::endl;
	return res;
}

NodeBase* RemoteNode::findSuccessor(unsigned int Id) {
    // TODO setup RPC call
    std::cout <<"mclient = " << m_client << std::endl;
    return m_client->findSuccessor(Id);
}

void RemoteNode::fixFingers() {
    std::cout <<"RemoteNode fixFingers(), invalid call" << std::endl;
}

NodeBase* RemoteNode::closestPrecedingNode(unsigned int Id) {;
    // TODO setup RPC call
}


void RemoteNode::stabilize() {
    std::cout <<"RemoteNode stabilize(), invalid call" << std::endl;
}


void RemoteNode::notify(NodeBase * remote) {
    // TODO setup RPC call
    m_client->notify(remote);
}

void RemoteNode::checkPredecessor() {
	PRINT_FUNC_NAME;
}

void RemoteNode::printTable() {
	m_table->printTable();
}