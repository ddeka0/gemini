#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceManager.hpp"
#include "ServerUtils/include/grpcAsyncServer.hpp"
#include "Chord/include/fingerTable.hpp"

class LocalNode :public NodeBase {
public:
    explicit LocalNode();
    virtual ~LocalNode();
    
    void initWithServer(std::shared_ptr<GrpcAsyncServer>) override;
    void initWithClient(std::shared_ptr<GrpcAsyncClient>) override;

    void setAddress(std::shared_ptr<Address>) override;
    std::shared_ptr<Address> getAddress() override;

    void addServices(uint8_t) override;
    void Start();

    void Get() override;
    unsigned int getId(unsigned int x = 0) override;

    NodeBase* getPredecessor() override;
    NodeBase* getSuccessor() override;

	void setSuccessor(NodeBase*) override;
	void setPredecessor(NodeBase*) override;

    NodeBase* findSuccessor(unsigned int) override;

	void fixFingers() override;
    
	void stabilize() override;

	void checkPredecessor() override;

	void notify(NodeBase*) override;

    NodeBase* closestPrecedingNode(unsigned int) override;

    void join(Address * remote_addr) override;

    void printTable() override;
    // void join() override;

	// methods to handle the client requests
    // all the functions that we can perform on this nod
protected:
private:
	bool m_active;
	ServiceManager m_srvcMgnr;
    std::shared_ptr<GrpcAsyncServer> m_server;
    std::shared_ptr<GrpcAsyncClient> m_client;
};