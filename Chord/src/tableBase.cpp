#include "tableBase.hpp"

TableBase::TableBase(){};

TableBase::~TableBase(){};

NodeBase*& TableBase::operator[](int idx) {
    return Map[idx];
}

void TableBase::setEntry(int idx,NodeBase* node) {
    // if(Map.find(idx) != Map.end()) {
    //     if(Map[idx] != nullptr) {
    //         delete Map[idx];
    //         Map[idx] = nullptr;
    //     }
    // }
    Map[idx] = node;
}

NodeBase* TableBase::getEntry(int idx) {
    if(Map.find(idx) != Map.end()) {
        return Map[idx];
    }else {
        return nullptr;
    }
}

void TableBase::printTable() {
    std::cout <<"Printing table from the base class" << std::endl;
}