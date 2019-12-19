#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceManager.hpp"

class LocalNode :public NodeBase {
public:
    LocalNode();
    ~LocalNode();
    void initialize(std::shared_ptr<GrpcAsyncServer>) override;
    void setAddress(std::shared_ptr<Address>) override;
    void addServices();
	// methods to handle the client requests
    // all the functions that we can perform on this nod
protected:
private:
	bool m_active;
	ServiceManager m_srvcMgnr;
    std::shared_ptr<GrpcAsyncServer> m_server;
};