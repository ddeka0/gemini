#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceManager.hpp"

class LocalNode :public NodeBase {
public:
    LocalNode(){};
    ~LocalNode(){};
	// methods to handle the client requests
    // all the functions that we can perform on this nod
protected:
private:
	bool m_active;
	ServiceManager m_srvcMgnr;
};