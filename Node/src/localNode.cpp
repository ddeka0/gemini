#include "localNode.hpp"

LocalNode::LocalNode(){
    // debug print
    std::cout << "LocalNode instance created" << std::endl;
    m_srvcMgnr.initialize();
};
LocalNode::~LocalNode(){
    // Debug print
    std::cout << "LocalNode instance destroyed" << std::endl;
};

void LocalNode::initialize(std::shared_ptr<GrpcAsyncServer> _server) {
    std::cout << "LocalNode Initialized"<< std::endl;
    m_server = _server;
}

/*
This function assigns address to the node
*/
void LocalNode::setAddress(std::shared_ptr<Address> _addr) {
    m_addr = _addr;
}

std::shared_ptr<Address> LocalNode::getAddress() {
    return m_addr;
}
/*
This function takes argument as bitmask
for example GET | PUT | DEL etc.
and the implementation perform service adding or deletion for this node
*/
void LocalNode::addServices() {
    // TODO add argument to the caller function
    std::cout <<"hh1" << std::endl;
    m_srvcMgnr.addService(GET_SERVICE);
}

void LocalNode::Start() {
    std::cout << __func__ << std::endl;
    m_server->Run();
}

void LocalNode::Get() {
    m_srvcMgnr.executeHandler();    
}