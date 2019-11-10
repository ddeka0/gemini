#pragma once
#include "nodeBase.hpp"
#include "Services/include/serviceBase.hpp"

class LocalNode :public nodeBase {
public:
    LocalNode(){};
    ~LocalNode(){};
    void initialize();
    // all the functions that we can perform on this node
protected:
private:
    std::vector<ServiceBase*> sv;
};