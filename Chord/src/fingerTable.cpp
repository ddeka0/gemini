#include "fingerTable.hpp"
FingerTable::FingerTable(){
    // Debug print
    PRINT_FUNC_NAME;
}
FingerTable::~FingerTable(){
    PRINT_FUNC_NAME;
}
void FingerTable::printTable() {
    for(auto const &x:Map) {
        std::cout << x.first <<" : "<<x.second->getAddress()->toString() << std::endl;
    }
}