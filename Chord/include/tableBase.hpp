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
    // This Map will be holder of NodeBase ptrs for indices from 0 to 2^n - 1
    std::map<int,NodeBase*> Map;

protected:
private:
};