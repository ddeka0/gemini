#include "getService.hpp"
GetService::GetService(){
    // Debug print
    std::cout << "GetService instance created" << std::endl;
};
GetService::~GetService(){
    // Debug print
    std::cout << "Getservie instance destroyed" << std::endl;
};

void GetService::Get() {
    // Debug print
    std::cout << "Get() called from GetService" << std::endl;
    // TODO implement the GET() function logic using generic methods in the
    // ServiceBase class
}