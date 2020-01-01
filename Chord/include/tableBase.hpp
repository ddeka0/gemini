#pragma once
#include "common.hpp"
#include "Node/include/nodeBase.hpp"

class TableBase {
public:
    TableBase();
    ~TableBase();
    void updateTable();
    void clearTable();
    NodeBase* getNodePtr();
    virtual NodeBase*& operator[](int);
    // these function supposed to be in FingerTable class
    // but for now although we are using FingerTable object in the code
    // that object is using TableBase methods only
    virtual void setEntry(int,NodeBase*);
    virtual NodeBase* getEntry(int);
    virtual void printTable();
    // This Map will be holder of NodeBase ptrs for indices from 0 to 2^n - 1
    std::map<int,NodeBase*> Map;

protected:
private:
};