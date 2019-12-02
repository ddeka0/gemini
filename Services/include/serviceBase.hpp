#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/tableBase.hpp"
#define NBITS 10
class ServiceBase {
public:
    ServiceBase();
    ~ServiceBase();
    void fixFingers();
    void stabilize();
    void checkPredecessor();
    void notify();
    NodeBase* findSuccessor();
    NodeBase* closestPrecedingNode(unsigned int);
protected:
private:
    TableBase * m_table;
};