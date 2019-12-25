#pragma once
#include "Node/include/nodeBase.hpp"
#include "Chord/include/common.hpp"
#include "protocol/test.grpc.pb.h" // this file will be generated 
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
	// This section of code might not compile
	// because we dont have compiled code for proto files yet
	// for example we dont have mathlib ready

	mathlib::Math::AsyncService service_;
	std::unique_ptr<ServerCompletionQueue> cq_;
	std::unique_ptr<Server> server_;
	NodeBase* m_pNode;
};


struct CallData {
    mathlib::Math::AsyncService* service;
    grpc::ServerCompletionQueue* cq;
};

// Base class used to cast the void* tags we get from
// the completion queue and call Proceed() on them.
class Call {
public:
    virtual void Proceed() = 0;
};

class AddCall : public Call {
public:
    explicit AddCall(CallData* data,GrpcAsyncServer * _server) : data_(data), responder_(&ctx_), 
		status_(CREATE),m_server(_server)  {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;

				data_->service->RequestAdd(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
				// Test
				m_server->getNode()->Get();
			}
			break;
			case PROCESS: {
				new AddCall(data_,m_server);

				int a = request_.a();
				int b = request_.b();
				response_.set_result(a+b);

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
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
	GrpcAsyncServer * m_server;
};

class SubCall : public Call {
public:
    explicit SubCall(CallData* data,GrpcAsyncServer *_server) : data_(data), responder_(&ctx_), 
		status_(CREATE),m_server(_server)  {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;
				data_->service->RequestSub(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
				
				// Test
				m_server->getNode()->Get();
			}
			break;
			case PROCESS: {
				new SubCall(data_,m_server);

				// call the methods from Services
				// m_server->getNode()->Get();
				
				int a = request_.a();
				int b = request_.b();
				response_.set_result(a - b);

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
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
	GrpcAsyncServer * m_server;
};

