#include <bits/stdc++.h>
#include <chrono>
#include <sys/time.h>
#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

using namespace std;
using namespace std::chrono;

#include "math.grpc.pb.h"


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;


class MathClient {
 public:
	explicit MathClient(std::shared_ptr<Channel> channel)
		: stub_(mathlib::Math::NewStub(channel)) {}
	
	int Add(int a,int b) {
		mathlib::MathRequest request;
		request.set_a(a);
		request.set_b(b);
		mathlib::MathReply reply;
		ClientContext context;
		grpc::CompletionQueue cq;
		grpc::Status status;
		std::unique_ptr<ClientAsyncResponseReader<mathlib::MathReply>> 
			rpc(stub_->PrepareAsyncAdd(&context, request, &cq));
		rpc->StartCall();
		rpc->Finish(&reply, &status, (void*)1);
		void* got_tag;
		bool ok = false;
		GPR_ASSERT(cq.Next(&got_tag, &ok));
		GPR_ASSERT(got_tag == (void*)1);
		GPR_ASSERT(ok);

		if (status.ok()) {
			return reply.result();
		} else {
			return -1; // need to improve
		}
	}

private:
	
	std::unique_ptr<mathlib::Math::Stub> stub_;

};


struct CallData {
    mathlib::Math::AsyncService* service;
    grpc::ServerCompletionQueue* cq;
};

class RemoteNode {
public:
	std::unique_ptr<MathClient> mclient;
	
	RemoteNode(std::string remote_address, std::string remote_port)
		: remote_address(remote_address),remote_port(remote_port) {

		mclient = std::unique_ptr<MathClient>(
			new MathClient(
				grpc::CreateChannel(
					remote_address+":"+remote_port, 
					grpc::InsecureChannelCredentials()
				)
			)
		);	
	}

private:
	std::string remote_address;
	std::string remote_port;
};

// this map stores the Contexts of the remote nodes known to this node
std::map<int,std::unique_ptr<RemoteNode>> Map; 


// Base class used to cast the void* tags we get from
// the completion queue and call Proceed() on them.
class Call {
public:
    virtual void Proceed() = 0;
};


// This is a custom class, we need to implement this to handle a specific method of rpc
// In this particular case the rpc method is Cmplx, class name could be anything btw
// But we need to make sure to call
class CmplxCall : public Call {
public:
    explicit CmplxCall(CallData* data) : data_(data), responder_(&ctx_), 
		status_(CREATE)  {
		Proceed();
	}
	void Proceed() {
		switch (status_) {
			case CREATE: {
				status_ = PROCESS;
				data_->service->RequestCmplx(&ctx_,&request_,&responder_, data_->cq,
						data_->cq, this);
			}
			break;
			case PROCESS: {
				new CmplxCall(data_);

				int a = request_.a();
				int b = request_.b();

				// Here we need to do an RPC call to a remote 
				int idx = 0;
				int x = Map[idx]->mclient->Add(a,b);

				response_.set_result(x + b);

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

	RemoteNode* node;
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
		CallData data{&service_,cq_.get()}; // data varible is taking ServerImpl's varibles
		new CmplxCall(&data); // a new instance of CmplxCall is associated with ServerImpl's varibles
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

	// these are two key variable of the ServerImpl
	// these varibles are shared by all the Custom classes of type "CmplxCall" corresponding to a 
	// rpc method
	
	mathlib::Math::AsyncService service_;
	std::unique_ptr<ServerCompletionQueue> cq_;
	
	std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
	ServerImpl server;
	// Map simulates the finger table of chord paper
	Map[0] = std::unique_ptr<RemoteNode>(new RemoteNode("localhost","50052"));
	server.Run();

	return 0;
}
