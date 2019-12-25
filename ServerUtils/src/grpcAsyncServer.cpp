#include "Node/include/nodeBase.hpp"
#include "grpcAsyncServer.hpp"
#include "Chord/include/address.hpp"

GrpcAsyncServer::GrpcAsyncServer() {
	std::cout << "GrpcAsyncServer instance Created" << std::endl;
}

GrpcAsyncServer::GrpcAsyncServer(NodeBase* node) {
	// Debug print
	m_pNode = node;
};
GrpcAsyncServer::~GrpcAsyncServer() {
	server_->Shutdown();
	cq_->Shutdown();
};

// we need to call this Run() method from ecterna module
void GrpcAsyncServer::Run() {
	std::string addr = m_pNode->getAddress()->toString();
	std::cout << addr << std::endl;
	std::string server_address(addr);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service_);
	cq_ = builder.AddCompletionQueue();
	server_ = builder.BuildAndStart();
	std::cout << "Server listening on " << server_address << std::endl;
	HandleRpcs();
}

void GrpcAsyncServer::HandleRpcs() {
	CallData data{&service_,cq_.get()};
	new AddCall(&data,this);
	new SubCall(&data,this);
	void* tag;
	bool ok;
	while (true) {
		GPR_ASSERT(cq_->Next(&tag, &ok));
		GPR_ASSERT(ok);
		if(tag == nullptr) {
			std::cout <<"someting wrong !"<<std::endl;
		}else {	
			static_cast<Call*>(tag)->Proceed();
		}
	}
}

NodeBase * GrpcAsyncServer::getNode() {
	return m_pNode;
}