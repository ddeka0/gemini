#pragma once
#include "serviceBase.hpp"
#include "getService.hpp"
enum e_ServiceType {
	NO_SERBVICE = 0,
	GET_SERVICE,
	PUT_SERVICE,
	MAX_SERVICE
};

class ServiceManager {
public:
	ServiceManager();
	~ServiceManager();
	void initialize();
	void addService(e_ServiceType service);
	void addService(uint8_t flag);
	void delService(e_ServiceType service);
	void updateService();

	void executeHandler();
	
protected:
private:
	std::vector<std::shared_ptr<ServiceBase>> m_serviceList;
};