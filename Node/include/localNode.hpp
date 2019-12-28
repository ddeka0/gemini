#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceManager.hpp"
#include "ServerUtils/include/grpcAsyncServer.hpp"

class LocalNode :public NodeBase {
public:
    LocalNode();
    virtual ~LocalNode();
    
    void initialize(std::shared_ptr<GrpcAsyncServer>) override;
    void setAddress(std::shared_ptr<Address>) override;
    std::shared_ptr<Address> getAddress() override;

    void addServices(uint8_t) override;
    void Start();

    void Get() override;
	// methods to handle the client requests
    // all the functions that we can perform on this nod
protected:
private:
	bool m_active;
	ServiceManager m_srvcMgnr;
    std::shared_ptr<GrpcAsyncServer> m_server;
};