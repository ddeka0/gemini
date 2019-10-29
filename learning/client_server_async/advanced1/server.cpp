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


#include "math.grpc.pb.h"


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;


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
		cout << "AddCall constructor called" << endl;
        on_done = [&](bool ok) { OnDone(ok); };
		proceed = [&]() { Proceed(); };
		// ctx_.AsyncNotifyWhenDone(&on_done);

		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;

				data_->service->RequestAdd(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
				cout <<"RequestAdd called"<<endl;
			}
			break;
			case PROCESS: {
				new AddCall(data_);

				int a = request_.a();
				int b = request_.b();
				response_.set_result(a+b);

				status_ = FINISH;
				responder_.Finish(response_,Status::OK,this);
				cout << "AddCall response sent" << endl;
			}
			break;
			case FINISH: {
				// finish_called_ = true;
				// if (on_done_called_)

				delete this;
				cout <<"AddCall decallocated"<<endl;
			}
			break;
			default:
				break;
		}
	}
	void OnDone(bool ok) {
		assert(ok);
		on_done_called_ = true;
		if (finish_called_)
			delete this;
		else
			status_ = FINISH;
	}

	std::function<void(void)> proceed;
	std::function<void(bool)> on_done;

private:
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
	bool finish_called_ = false;
	bool on_done_called_ = false;
};

class SubCall : public Call {
public:
    explicit SubCall(CallData* data) : data_(data), responder_(&ctx_), 
		status_(CREATE)  {
		cout << "SubCall constructor called" << endl;
        on_done = [&](bool ok) { OnDone(ok); };
		proceed = [&]() { Proceed(); };
		// ctx_.AsyncNotifyWhenDone(&on_done);

		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;
				data_->service->RequestSub(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
				cout<<"RequestSub called"<<endl;
			}
			break;
			case PROCESS: {
				new SubCall(data_);

				int a = request_.a();
				int b = request_.b();
				response_.set_result(a - b);

				status_ = FINISH;
				responder_.Finish(response_,Status::OK,this);
				cout << "SubCall response sent" << endl;
			}
			break;
			case FINISH: {
				// finish_called_ = true;
				// if (on_done_called_)
				delete this;
				cout<<"SubCall decallocated"<<endl;
			}
			break;
			default:
				break;
		}
	}
	void OnDone(bool ok) {
		assert(ok);
		on_done_called_ = true;
		if (finish_called_)
			delete this;
		else
			status_ = FINISH;
	}

	std::function<void(void)> proceed;
	std::function<void(bool)> on_done;

private:
    CallData* data_;
    grpc::ServerContext ctx_;
	grpc::ServerAsyncResponseWriter<mathlib::MathReply> responder_;

	mathlib::MathRequest request_;
	mathlib::MathReply response_;

	enum CallStatus { CREATE, PROCESS, FINISH };
	CallStatus status_;
	bool finish_called_ = false;
	bool on_done_called_ = false;
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
			static_cast<Call*>(tag)->Proceed();
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
