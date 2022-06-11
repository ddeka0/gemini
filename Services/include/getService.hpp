#pragma once
#include "serviceBase.hpp"
class GetService : public ServiceBase {
public:
	GetService();
	virtual ~GetService();

	void Get() override;
protected:
private:
};