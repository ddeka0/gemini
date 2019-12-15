#include <bits/stdc++.h>
#include <thread>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "proto_lib/math.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;


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

	int Sub(int a,int b) {
		mathlib::MathRequest request;
		request.set_a(a);
		request.set_b(b);
		mathlib::MathReply reply;
		ClientContext context;
		grpc::CompletionQueue cq;
		grpc::Status status;
		std::unique_ptr<ClientAsyncResponseReader<mathlib::MathReply>> 
			rpc(stub_->PrepareAsyncSub(&context, request, &cq));
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

int main(int argc, char** argv) {

	// single client test
	// MathClient mclient(grpc::CreateChannel(
	// 	"localhost:50051", grpc::InsecureChannelCredentials()));
		
	// int x = -1,y = 1;
	// x = mclient.Add(1,2);  // The actual RPC call!
	// y = mclient.Sub(3,5);
	// std::cout << "Received:	 " << x <<" and "<< y << std::endl;

	// multi client test
	int num_clients = 200;
	std::vector<std::thread> v;
	for(int i = 0;i<num_clients;i++) {
		std::thread t([]() {
			MathClient mclient(grpc::CreateChannel(
			"localhost:50051", grpc::InsecureChannelCredentials()));

			int x = mclient.Add(1,2);
			int y = mclient.Sub(3,5);
			std::cout << "Received:	 " << x <<" and "<< y << std::endl;
		});
		v.push_back(std::move(t));
	}
	
	std::for_each(v.begin(), v.end(), [](std::thread & t){ t.join(); });
	return 0;
}
