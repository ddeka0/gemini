#pragma once
#include "common.hpp"
#include "tableBase.hpp"
class FingerTable : public TableBase {
public:
    FingerTable();
    ~FingerTable();
    void printTable() override;
protected:
private:
};