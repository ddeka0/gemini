#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/address.hpp"
// #include "ServerUtils/include/grpcAsyncServer.hpp"
/*
This is a parent node class.
It should have:
1. Address
2. 
*/
class GrpcAsyncServer;

class NodeBase {
public:
	NodeBase();
	~NodeBase();
	virtual void initialize(std::shared_ptr<GrpcAsyncServer>) = 0; // pure vf
	virtual std::shared_ptr<Address> getAddress() = 0;
	
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

	virtual void setAddress(std::shared_ptr<Address>) = 0;
	virtual void Start() = 0;
	virtual unsigned int getId(unsigned int x = 0); // default offset value

	virtual void Get();
	// member varibles
protected:
	std::shared_ptr<Address> m_addr;
private:
};