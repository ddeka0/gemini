#include "Node/include/remoteNode.hpp"
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

	// Prepare FindSuccessor RPC ///////////////////////////////////////////////
	auto initAsyncReq_FindSuccessor = [this](ServerContext * ctx, 
		chordMsg::Id* request, 
		grpc::ServerAsyncResponseWriter<chordMsg::NodeAddr>* responder,
		grpc::CompletionQueue* call_cq,
		grpc::ServerCompletionQueue* notfiy_cq,void * tag) {
			this->service_.RequestFindSuccessor(ctx,request,responder,call_cq,notfiy_cq,tag);
		};

	auto invokerRpc_FindSuccessor = [this](chordMsg::Id request) {
		std::cout <<"RPC request came from remote node" << std::endl;
		
		NodeBase* node = this->m_pNode->findSuccessor(request.id());
		chordMsg::NodeAddr resp;
		resp.set_ip_addr(node->getAddress()->getIpAddr());
		resp.set_port(node->getAddress()->getPort());
		if(node != this->getNode()) {
			delete node;
		}
		return resp;
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
	////////////////////////////////////////////////////////////////////////////


	// Prepare getPredecessor //////////////////////////////////////////////////
	auto initAsyncReq_getPredecessor = [this](ServerContext * ctx, 
		chordMsg::Empty* request, 
		grpc::ServerAsyncResponseWriter<chordMsg::NodeAddr>* responder,
		grpc::CompletionQueue* call_cq,
		grpc::ServerCompletionQueue* notfiy_cq,void * tag) {
			this->service_.RequestgetPredecessor(ctx,request,responder,call_cq,
				notfiy_cq,tag);
		};

	auto invokerRpc_getPredecessor = [this](chordMsg::Empty request) {
		std::cout <<"getPredecessor RPC request came from remote node" << std::endl;
		
		NodeBase* node = this->m_pNode->getPredecessor();
		chordMsg::NodeAddr resp;
		resp.set_ip_addr(node->getAddress()->getIpAddr());
		resp.set_port(node->getAddress()->getPort());
		return resp;
	};
	// Create a new instance of RpcStateMgmt dedicated for FindSuccessor RPC
	new RpcStateMgmt<chordMsg::ChordService::AsyncService,chordMsg::Empty, 
	chordMsg::NodeAddr,decltype(initAsyncReq_getPredecessor),
	decltype(invokerRpc_getPredecessor)>
	(
		&service_,
		cq_.get(),
		this,
		initAsyncReq_getPredecessor,
		invokerRpc_getPredecessor
	);
	////////////////////////////////////////////////////////////////////////////


	// Prepare notify RPC //////////////////////////////////////////////////////
	auto initAsyncReq_notify = [this](ServerContext * ctx, 
		chordMsg::NodeAddr* request, 
		grpc::ServerAsyncResponseWriter<chordMsg::Empty>* responder,
		grpc::CompletionQueue* call_cq,
		grpc::ServerCompletionQueue* notfiy_cq,void * tag) {
			this->service_.RequestNotify(ctx,request,responder,call_cq,notfiy_cq,tag);
		};

	auto invokerRpc_notify = [this](chordMsg::NodeAddr request) {
		std::cout <<"notify RPC request came from remote node" << std::endl;
		

		RemoteNode *node = new RemoteNode();
		std::string ipAddr = request.ip_addr();
		std::string port = request.port();
		node->setAddress(
			std::shared_ptr<Address>(new Address(ipAddr,port))
		);
		std::shared_ptr<GrpcAsyncClient> client = std::shared_ptr<GrpcAsyncClient>(
			new GrpcAsyncClient(node)
		);  
		node->initWithClient(client);
		this->m_pNode->notify(node); // does not return anything as of now
		
		chordMsg::Empty resp;
		return resp;
	};
	// Create a new instance of RpcStateMgmt dedicated for notify RPC
	new RpcStateMgmt<chordMsg::ChordService::AsyncService,chordMsg::NodeAddr, 
	chordMsg::Empty,decltype(initAsyncReq_notify),
	decltype(invokerRpc_notify)>
	(
		&service_,
		cq_.get(),
		this,
		initAsyncReq_notify,
		invokerRpc_notify
	);
	////////////////////////////////////////////////////////////////////////////
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