#include "tableBase.hpp"

TableBase::TableBase(){};

TableBase::~TableBase(){};

NodeBase*& TableBase::operator[](int idx) {
    return Map[idx];
}