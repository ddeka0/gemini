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
    m_table = std::shared_ptr<TableBase>(new FingerTable());
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
    std::cout <<"hh1" << std::endl;
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
    return (*m_table)[0];
}

void LocalNode::setSuccessor(NodeBase * node) {
    PRINT_FUNC_NAME;
    (*m_table)[0] = node;
}
void LocalNode::setPredecessor(NodeBase * node) {
    PRINT_FUNC_NAME;
    m_predecessor = node;
}

unsigned int LocalNode::getId(unsigned int x) {
    // TODO implement consistent hash with x
    return 123;
}

NodeBase* LocalNode::findSuccessor(unsigned int Id) {
	if(inrange(Id,this->getId(),this->getId()/* this is wrong*/) and
		(this->getId() != this->getId()) and 
		(Id != this->getId())) {

		return this->getSuccessor(); // this is wrong , we need to return m_node->successor();	
	}else {
		NodeBase* remote = closestPrecedingNode(Id); // TODO smart ptr
		if(this->getId() != remote->getId()) {
			// remote.getWhatServiceIsThis().findSuccessor(Id);
			// therefore a node class must have a pointer to a specific service
			// it will be having pointers to ServiceBase class.
			// becuase we are inside ServiceBase class method, there must be 
			// some mechanism to get the hold of remote's service objects

		}else{
			return this; // TODO good macro for this like SELF, THIS_NODE etc.
		}

	}
}

void LocalNode::fixFingers() {
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
		((*m_table)[nxt - 1]) = this->findSuccessor(this->getId(1<<(nxt - 1)));
		// m_table[nxt - 1] = 
		// TODO make inclide.hpp header file for all such such function
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

NodeBase* LocalNode::closestPrecedingNode(unsigned int Id) {
	for(int idx = NBITS - 1;idx >=0 ;idx--) {
		if(m_table->Map[idx]->getId() != -1 and
			inrange(m_table->Map[idx]->getId(),this->getId(),Id) and
			(m_table->Map[idx]->getId() != this->getId()) and 
			Id != m_table->Map[idx]->getId()) 
		{
			return m_table->Map[idx];
		} 
	}
	return this; // TODO a good macro for this like SELF, THIS_NODE etc.
}

/* This function will run in a separate thread and run for ever
*/
void LocalNode::stabilize() {
	// infinite loop
	PRINT_FUNC_NAME;
	while(true) {
		if(nullptr != this->getPredecessor()) {
			std::cout << "Not null predecessor" << std::endl;
		}
		if(nullptr != this->getSuccessor()) {
			std::cout << "Not null predecessor" << std::endl;
		}
		// my successor is pointing to myself, that means I were the first node
		// and then my predecesor is not null , i.e. pointing to some other node(x)
		// what does this mean ? it means my successor should also point to x
		// simultaneus node addition with the same remote and concurrency 
		// will be handled later
		auto suc = this->getSuccessor(); // suc is of type NodeBase *
		if(nullptr != suc and nullptr != this->getPredecessor()) {
			(*m_table)[0] = this->getPredecessor();
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
    }
    // some TODO task for key transfer from this node the new predecessor
}