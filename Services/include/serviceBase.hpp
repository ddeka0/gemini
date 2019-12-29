/*
An array of servieBase class pointers will be a member of a node instance
Therefore a node can easily access this service objects as long as we provide 
some public apis. 
But how Service class access access its node ?
Therefore we must need to store the pointer to to node instance in service 
class also
*/
#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/fingerTable.hpp"
#define NBITS 10
class ServiceBase {
public:
    ServiceBase();
    ~ServiceBase();
    void initialize(NodeBase*); // CHECK what type pointer to pass
    // void fixFingers();
    // void stabilize();
    // void checkPredecessor();
    // void notify();
    // NodeBase* findSuccessor(unsigned int);
    // NodeBase* closestPrecedingNode(unsigned int);

    // test get function
    virtual void Get();


protected:
private:
    // m_table holds the finger table pointer
    TableBase* m_table; // ServiceBase class does not owns this table
    NodeBase * m_node = nullptr; // Service Base class dont own this pointer
};