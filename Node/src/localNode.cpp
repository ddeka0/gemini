#include "localNode.hpp"
#include "remoteNode.hpp"
#include "Chord/include/utils.hpp" // for hashing

/* Constructor of LocalNode class
*/
LocalNode::LocalNode() {
	PRINT_FUNC_NAME;
    // debug print
    std::cout << "LocalNode instance created" << std::endl;
    m_srvcMgnr.initialize();
	m_table = std::shared_ptr<TableBase>(new FingerTable());

};

/* This function joins this node to the to exisiting ring with the 
* help of a remote node
*/
// def join(self, RemoteAddress=None):
// 	if RemoteAddress:
// 		remoteInstance = RemoteNode(RemoteAddress)
// 		self._finger[0] = remoteInstance.findSuccessor(self.getIdentifier())
// 	else:
// 		self._finger[0] = self  # fot the node-0
void LocalNode::join(Address * remote_addr) {
	if("none:none" != remote_addr->toString()) {
		// remoteInstance = RemoteNode(RemoteAddress)
		// self._finger[0] = remoteInstance.findSuccessor(self.getIdentifier())
		RemoteNode remoteInstance;
		auto rnode = remoteInstance.findSuccessor(this->getId());
		m_table->setEntry(0,rnode);
		
	}else {
		// set the successor to myself, or this node
		// (*m_table)[0] = this;
		m_table->setEntry(0,this);
	}
}
/* Destructor of LocalNode class
*/
LocalNode::~LocalNode() {
    // Debug print
    std::cout << "LocalNode instance destroyed" << std::endl;
};

void LocalNode::initWithServer(std::shared_ptr<GrpcAsyncServer> _server) {
    std::cout << "LocalNode Initialized"<< std::endl;
    m_server = _server;
}

void LocalNode::initWithClient(std::shared_ptr<GrpcAsyncClient> _client) {
    std::cout << "LocalNode Initialized"<< std::endl;
    m_client = _client;
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
void LocalNode::addServices(uint8_t flag) {
    PRINT_FUNC_NAME;
    // TODO add argument to the caller function
    m_srvcMgnr.addService(flag);
}

void LocalNode::Start() {
    PRINT_FUNC_NAME;
    m_server->Run();
}

void LocalNode::Get() {
    PRINT_FUNC_NAME;
    m_srvcMgnr.executeHandler();    
}

NodeBase* LocalNode::getPredecessor() {
    PRINT_FUNC_NAME;
    return m_predecessor;
}

NodeBase* LocalNode::getSuccessor() {
    PRINT_FUNC_NAME;
    // return (*m_table)[0];
	return m_table->getEntry(0);
}

void LocalNode::setSuccessor(NodeBase * node) {
    PRINT_FUNC_NAME;
	m_table->setEntry(0,node);
}
void LocalNode::setPredecessor(NodeBase * node) {
    PRINT_FUNC_NAME;
    m_predecessor = node;
}

unsigned int LocalNode::getId(unsigned int x) {
	// std::cout<<__func__<<std::endl;
	unsigned int res = sha256(m_addr->toString()) + x;
	// std::cout <<"res = "<<res<<std::endl;
	return res;
}

NodeBase* LocalNode::findSuccessor(unsigned int Id) {
	if(inrange(Id,this->getId(),this->getSuccessor()->getId()) and
		(this->getId() != this->getSuccessor()->getId()) and 
		(Id != this->getId())) {

		return this->getSuccessor();	
	}else {
		auto remote = closestPrecedingNode(Id);
		if(this->getId() != remote->getId()) {
			return remote->findSuccessor(Id);
		}else{
			return this;
		}

	}
}

void LocalNode::fixFingers() {
	PRINT_FUNC_NAME;
	bool running = true;
	int nxt = 0;
	while(running) {
		nxt++;
		// TODO place NBITS in proper location
		if(nxt > NBITS) {
			nxt = 1;
		}

		m_table->setEntry(nxt - 1,this->findSuccessor(this->getId(1<<(nxt - 1))));

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		// m_table->printTable();
		//this->printTable();
	}
}

NodeBase* LocalNode::closestPrecedingNode(unsigned int Id) {
	PRINT_FUNC_NAME;
	for(int idx = NBITS - 1;idx >=0 ;idx--) {
		if( m_table->getEntry(idx) != nullptr and
			inrange(m_table->getEntry(idx)->getId(),this->getId(),Id) and
			(m_table->getEntry(idx)->getId() != this->getId()) and 
			Id != m_table->getEntry(idx)->getId()) 
		{
			return m_table->getEntry(idx);
		} 
	}
	return this;
}

/* This function will run in a separate thread and run for ever
*/
void LocalNode::stabilize() {
	// infinite loop
	PRINT_FUNC_NAME;
	while(true) {
		std::string lg("");
		if(nullptr != this->getPredecessor()) {
			lg += "Predecessor = " + getPredecessor()->getAddress()->toString(); 
		}
		if(nullptr != this->getSuccessor()) {
			lg += " | Successor = " + getSuccessor()->getAddress()->toString();
		}
		std::cout <<getId()<<" "<<lg << std::endl;
		// my successor is pointing to myself, that means I were the first node
		// and then my predecesor is not null , i.e. pointing to some other node(x)
		// what does this mean ? it means my successor should also point to x
		// simultaneus node addition with the same remote and concurrency 
		// will be handled later
		auto suc = this->getSuccessor(); // suc is of type NodeBase *
		if(suc == nullptr) {
			std::cout << "successor is not set" << std::endl;
		}
		if(nullptr != suc and nullptr != this->getPredecessor()) {
			// (*m_table)[0] = this->getPredecessor();
			m_table->setEntry(0,this->getPredecessor());
		}else {

			auto pred = suc->getPredecessor();

			if(nullptr != pred and inrange(pred->getId(),this->getId(),suc->getId()) and 
				(this->getId() != suc->getId()) and
				(pred->getId() != this->getId()) and
				(pred->getId() != suc->getId()))
			{
				this->setSuccessor(pred);
			}
		}
		this->getSuccessor()->notify(this);
		// TODO make inclide.hpp header file for all such such function
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


void LocalNode::notify(NodeBase * remote) {
    PRINT_FUNC_NAME;
    if( (this->getPredecessor() == nullptr or this->getPredecessor() == this) or
    ( (inrange(remote->getId(),this->getPredecessor()->getId(),this->getId())) and
    (this->getPredecessor()->getId() != this->getId()) and
    (remote->getId() != this->getSuccessor()->getId()) and
    (remote->getId() != this->getId()) ) )
    {

        this->m_predecessor = remote;
    }else {
		std::cout <<"Somewhere else" << std::endl;
	}
    // some TODO task for key transfer from this node the new predecessor
}

void LocalNode::checkPredecessor() {
	PRINT_FUNC_NAME;
}

void LocalNode::printTable() {
	m_table->printTable();
}