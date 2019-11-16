#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/tableBase.hpp"

class ServiceBase {
public:
    ServiceBase();
    ~ServiceBase();
    void fixFingers();
    void stabilize();
    void checkPredecessor();
    void notify();
    void findSuccessor();
    void closestPrecedingNode();
protected:
private:
    TableBase * m_table;
};