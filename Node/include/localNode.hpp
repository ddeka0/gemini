#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceManager.hpp"

class LocalNode :public NodeBase {
public:
    LocalNode(){};
    ~LocalNode(){};
    // all the functions that we can perform on this nod
protected:
private:
	bool m_active;
	ServiceManager m_srvcMgnr;
};