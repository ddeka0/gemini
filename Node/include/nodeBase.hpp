#pragma once
#include "Chord/include/common.hpp"
#include "Chord/include/address.hpp"
// #include "Chord/include/tableBase.hpp"

class GrpcAsyncServer;
class TableBase;

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
	virtual void addServices(uint8_t) = 0;


	virtual void setAddress(std::shared_ptr<Address>) = 0;
	virtual void Start() = 0;
	virtual unsigned int getId(unsigned int x = 0); // default offset value

	virtual NodeBase* getPredecessor() = 0; // this could lead to a RPC call
											// or local function call
	virtual NodeBase* getSuccessor() = 0; // this could lead to a RPC call
											// or local function call

	virtual void setSuccessor(NodeBase*) = 0;
	virtual void setPredecessor(NodeBase*) = 0;

	virtual NodeBase* findSuccessor(unsigned int) = 0;

	virtual void fixFingers() = 0;
    
	virtual void stabilize() = 0;

	virtual void checkPredecessor() = 0;

	virtual void notify(NodeBase*) = 0;

    virtual NodeBase* closestPrecedingNode(unsigned int) = 0;

	virtual void join(Address * remote_addr) = 0;

	virtual void printTable() = 0;

	// virtual void join() = 0;
	
	virtual void Get();
	// member varibles
	std::shared_ptr<TableBase> m_table; // this table is common to all the 
										// services for a node
										// therefore we want a single instance
										// of it 
	NodeBase * m_predecessor = nullptr;
protected:
	std::shared_ptr<Address> m_addr;
private:
};