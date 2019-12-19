#pragma once
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
	explicit AddCall(CallData*);
	void Proceed();
private:
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
};

class SubCall : public Call {
public:
	explicit SubCall(CallData*);
	void Proceed();
private:
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
};

class GrpcAsyncServer {
public:
	GrpcAsyncServer();
	~GrpcAsyncServer();
	void Run();
protected:
private:
	void HandleRpcs();
	// This section of code might not compile
	// because we dont have compiled code for proto files yet
	// for example we dont have mathlib ready

	mathlib::Math::AsyncService service_;
	std::unique_ptr<ServerCompletionQueue> cq_;
	std::unique_ptr<Server> server_;

};