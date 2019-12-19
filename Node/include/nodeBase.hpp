#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/address.hpp"
#include "ServerUtils/include/grpcAsyncServer.hpp"
/*
This is a parent node class.
It should have:
1. Address
2. 
*/
class NodeBase {
public:
	NodeBase(){};
	~NodeBase(){};
	NodeBase(const Address & addr);
	virtual void initialize(std::shared_ptr<GrpcAsyncServer>);
	virtual Address * getAddress();
	
	/*
		We could also declare a setAddress function
		as follows:
			void setAddress(Address&) : which accepts lvalue ref
			void setAddress(Address&&) : which accepts rvalue ref
		more:
						   template<typename T>
			we can also do void setAddress(T&&)
		check this options later
	*/

	virtual void setAddress(std::shared_ptr<Address>);
	virtual unsigned int getId(unsigned int x = 0); // default offset value
	
	// member varibles
protected:
	std::shared_ptr<Address> m_addr;
private:
};