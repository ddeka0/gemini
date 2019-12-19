#include "serviceManager.hpp"
ServiceManager::ServiceManager(){};
ServiceManager::~ServiceManager(){};
void ServiceManager::initialize() {
	m_serviceList.reserve(MAX_SERVICE);
	for(auto & s_ptr:m_serviceList) {
		s_ptr = nullptr;
	}
}

void ServiceManager::addService(e_ServiceType service) {
	if(service == GET_SERVICE) {
		std::shared_ptr<ServiceBase> _srvc = std::shared_ptr<ServiceBase>(new GetService());
		m_serviceList[GET_SERVICE] = _srvc;
	}else if(service == PUT_SERVICE) {
		// TODO add put serice logic
	}
}