#include "serviceManager.hpp"
ServiceManager::ServiceManager(){};
ServiceManager::~ServiceManager(){};
void ServiceManager::initialize() {
	serviceList.reserve(MAX_SERVICE);
	for(auto & s_ptr:serviceList) {
		s_ptr = nullptr;
	}
}