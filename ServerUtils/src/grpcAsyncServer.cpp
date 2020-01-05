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

void GrpcAsyncServer::InitiateRpcRequest() {
	// Prepare FindSuccessor RPC
	auto initAsyncReq_FindSuccessor = [this](ServerContext * ctx, 
		chordMsg::Id* request, 
		grpc::ServerAsyncResponseWriter<chordMsg::NodeAddr>* responder,
		grpc::CompletionQueue* call_cq,
		grpc::ServerCompletionQueue* notfiy_cq,void * tag) {
			this->service_.RequestFindSuccessor(ctx,request,responder,call_cq,notfiy_cq,tag);
		};

	auto invokerRpc_FindSuccessor = [this](unsigned int Id) {
		return this->m_pNode->findSuccessor(Id);
	};
	// Create a new instance of RpcStateMgmt dedicated for FindSuccessor RPC
	new RpcStateMgmt<chordMsg::ChordService::AsyncService,chordMsg::Id, 
	chordMsg::NodeAddr,decltype(initAsyncReq_FindSuccessor),
	decltype(invokerRpc_FindSuccessor)> 
	(
		&service_,
		cq_.get(),
		this,
		initAsyncReq_FindSuccessor,
		invokerRpc_FindSuccessor
	);

	// Still to add other RPC handlers
}
void GrpcAsyncServer::HandleRpcs() {
	InitiateRpcRequest();
	void* tag;
	bool ok;
	while (true) {
		GPR_ASSERT(cq_->Next(&tag, &ok));
		GPR_ASSERT(ok);
		if(tag == nullptr) {
			std::cout <<"someting wrong !"<<std::endl;
		}else {	
			static_cast<RpcStateMgmtIntf*>(tag)->Proceed();
		}
	}
}

NodeBase * GrpcAsyncServer::getNode() {
	return m_pNode;
}