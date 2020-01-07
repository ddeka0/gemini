#pragma once
#include "Node/include/nodeBase.hpp"
#include "Chord/include/common.hpp"
#include "protocol/messages.grpc.pb.h" // this file will be generated 
// only when you execute the makefile inside protocol folder


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;



#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

class GrpcAsyncServer {
public:
	GrpcAsyncServer();
	~GrpcAsyncServer();
	GrpcAsyncServer(NodeBase*);
	void Run();
	NodeBase * getNode();
protected:
private:
	void HandleRpcs();
	// This function will Prepare and Create new instances of RpcStateMgmt for 
	// handling each RPC method for the clients
	void InitiateRpcRequest();
	// This section of code might not compile
	// because we dont have compiled code for proto files yet
	// for example we dont have mathlib ready

	chordMsg::ChordService::AsyncService service_;
	std::unique_ptr<ServerCompletionQueue> cq_;
	std::unique_ptr<Server> server_;
	NodeBase* m_pNode;
	
};


// Base class used to cast the void* tags we get from
// the completion queue and call Proceed() on them.



class RpcStateMgmtIntf {
public:
    virtual void Proceed() = 0;
};

template<class Service, class Request, class Reply,  typename InitAsyncReqFunc, 
typename InvokerRpcFunc>
class RpcStateMgmt : public RpcStateMgmtIntf {
public:
    explicit RpcStateMgmt(Service* service, ServerCompletionQueue* cq,
			GrpcAsyncServer * _server, InitAsyncReqFunc& initAsyncReqfunc, 
			InvokerRpcFunc & invokerRpcfunc)
		: service_(service),
		cq_(cq), 
		responder_(&ctx_), 
		status_(CREATE),
		m_server(_server),  
		initAsyncReqfunc_(initAsyncReqfunc),
		invokerRpcfunc_(invokerRpcfunc) {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;
				initAsyncReqfunc_(&ctx_,&request_,&responder_,cq_,cq_,this);
				// // folling line is a test code
				// // m_server->getNode()->Get();
				// invokerRpcfunc_(100);
			}
			break;
			case PROCESS: {

				new RpcStateMgmt<Service,Request,Reply,InitAsyncReqFunc,
				InvokerRpcFunc>
				(service_, cq_, m_server,initAsyncReqfunc_, invokerRpcfunc_);


				// TODO call the invokerRpcfunc_
				// int a = request_.a();
				// int b = request_.b();
				// response_.set_result(a+b);
				invokerRpcfunc_(0); // 0 is wrong as of now


				status_ = FINISH;
				
				responder_.Finish(response_,Status::OK,this);
				
			}
			break;
			case FINISH: {
				
				GPR_ASSERT(status_ == FINISH);
				delete this;
				
			}
			break;
			default:
				break;
		}
	}
private:
	Service* service_;
	grpc::ServerCompletionQueue* cq_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<Reply> responder_;

	Request request_;
	Reply response_;

	InitAsyncReqFunc initAsyncReqfunc_; // for example RequestSub ptr
	InvokerRpcFunc invokerRpcfunc_; // GET or PUT service fuction ptr

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
	GrpcAsyncServer * m_server;
};


