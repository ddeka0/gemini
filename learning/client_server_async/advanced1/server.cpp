#include <bits/stdc++.h>
#include <variant>
#include <time.h>
#include <sys/time.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>
#include <functional>
#include <future>
#include <cmath>
#include <thread>
using namespace std;
using namespace std::chrono;

#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>


#include "proto_lib/math.grpc.pb.h"


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;


// main code starts from this position -------------------------

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
    explicit AddCall(CallData* data) : data_(data), responder_(&ctx_), 
		status_(CREATE)  {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;

				data_->service->RequestAdd(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
			}
			break;
			case PROCESS: {
				new AddCall(data_);

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
};

class SubCall : public Call {
public:
    explicit SubCall(CallData* data) : data_(data), responder_(&ctx_), 
		status_(CREATE)  {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;
				data_->service->RequestSub(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
			}
			break;
			case PROCESS: {
				new SubCall(data_);

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
};


class ServerImpl final {
public:
	~ServerImpl() {
		server_->Shutdown();
		// Always shutdown the completion queue after the server.
		cq_->Shutdown();
	}

	void Run() {
		std::string server_address("0.0.0.0:50051");
		ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&service_);
		cq_ = builder.AddCompletionQueue();
		server_ = builder.BuildAndStart();
		std::cout << "Server listening on " << server_address << std::endl;
		HandleRpcs();
	}

 private:
	void HandleRpcs() {
		CallData data{&service_,cq_.get()};
		new AddCall(&data);
		new SubCall(&data);
		void* tag;
		bool ok;
		while (true) {
			GPR_ASSERT(cq_->Next(&tag, &ok));
			GPR_ASSERT(ok);
			if(tag == nullptr) {
				cout <<"someting wrong !"<<endl;
			}else {	
				static_cast<Call*>(tag)->Proceed();
			}
		}
	}

	mathlib::Math::AsyncService service_;
	std::unique_ptr<ServerCompletionQueue> cq_;
	std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
	ServerImpl server;
	server.Run();

	return 0;
}
