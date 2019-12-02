#pragma once
#include "common.hpp"
#include "Node/include/nodeBase.hpp"
class TableBase {
public:
    TableBase(){};
    ~TableBase(){};
    void updateTable(){};
    void clearTable();
    NodeBase* getNodePtr();
    std::map<int,NodeBase*> Map;
protected:
private:
};