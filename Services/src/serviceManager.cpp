#include "serviceManager.hpp"
ServiceManager::ServiceManager(){
	// Debug print
	std::cout << "ServiceManager instance created" << std::endl;
};
ServiceManager::~ServiceManager(){
	// Debug print
	// m_serviceList.clear();
	std::cout << "Service Manager instance Destroyed" << std::endl;
};
/*
This function reserves space for the serviceList inside serviceManager object
*/
void ServiceManager::initialize() {
	m_serviceList.resize(MAX_SERVICE);
	for(auto & s_ptr:m_serviceList) {
		s_ptr = nullptr;
	}
	std::cout << __FILE__<<" : "<<__func__<<" Size of m_serviceList = "<<m_serviceList.size()<<std::endl;
}
/*
This function takes service names and allocated service objects to the list
of services of servvicemanager, this servicemanager will be used by the 
*/
void ServiceManager::addService(e_ServiceType service) {
	PRINT_FUNC_NAME;
	if(service == GET_SERVICE) {
		std::shared_ptr<ServiceBase> _srvc = std::shared_ptr<ServiceBase>(new GetService());
		m_serviceList[GET_SERVICE] = _srvc;
	}else if(service == PUT_SERVICE) {
		// TODO add put serice logic
	}
}

void ServiceManager::addService(uint8_t flag) {
	PRINT_FUNC_NAME;
	if(flag & GET_SERVICE_FLAG) {
		std::shared_ptr<ServiceBase> _srvc = std::shared_ptr<ServiceBase>(new GetService());
		m_serviceList[GET_SERVICE] = _srvc;
	}else if(flag & PUT_SERVICE_FLAG) {
		// TODO add put serice logic
	}
}

void ServiceManager::executeHandler() {
	// as of now it is executing GET() handler by default
	m_serviceList[GET_SERVICE]->Get();
}