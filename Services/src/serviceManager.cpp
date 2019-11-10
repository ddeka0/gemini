#include "serviceManager.hpp"
void ServiceManager::initialize() {
	serviceList.reserve(MAX_SERVICE);
	for(auto & s_ptr:serviceList) {
		s_ptr = nullptr;
	}
}