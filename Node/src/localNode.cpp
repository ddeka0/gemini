#include "localNode.hpp"

LocalNode::LocalNode(){};
LocalNode::~LocalNode(){};

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

void LocalNode::addServices() {
    // TODO add argument to the caller function
    m_srvcMgnr.addService(GET_SERVICE);
}