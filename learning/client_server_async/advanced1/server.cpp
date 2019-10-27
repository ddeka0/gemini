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

// Simple POD struct used as an argument wrapper for calls
struct CallData {
	mathlib::Math::AsyncService* service;
    grpc::ServerCompletionQueue* cq;
    // other stuff
};

// Base class used to cast the void* tags we get from
// the completion queue and call Proceed() on them.
class Call {
public:
    virtual void Proceed(bool ok) = 0;
};

class AddCall {
public:

private:
}

class SubCall : public Call {
public:

private:
}


class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run() {
    std::string server_address("0.0.0.0:50051");

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    // Proceed to the server's main loop.
    HandleRpcs();
  }

 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(Greeter::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      // std::cout <<"Constructor Called by TID = " << std::this_thread::get_id() << std::endl;
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;

        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
		service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_,
                                  this);
        // std::cout <<"RequestSayHello called by TID = " << std::this_thread::get_id() << std::endl;
      } else if (status_ == PROCESS) {
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        // std::cout << "new CallData Created! by TID = " << std::this_thread::get_id() <<std::endl;
        new CallData(service_, cq_);

        // The actual processing.
        std::string prefix("Hello ");
		{
			int x = 0;
			int y = 0;
			while(x++ <= 100000000 ) { // takes around 1 sec to complete
				y = y + 10;
				y = y - 10;
			}
		}
        reply_.set_message(prefix + request_.name());

        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
		    std::cout << "Replied by TID = " << std::this_thread::get_id() <<std::endl;
      } else {
        std::cout <<"deallocate done! by TID = "<< std::this_thread::get_id()  << std::endl;
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }

   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    Greeter::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    HelloRequest request_;
    // What we send back to the client.
    HelloReply reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<HelloReply> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    auto x = new CallData(&service_, cq_.get());
	  Thpool pool;
    void* tag;  // uniquely identifies a request.
    bool ok;
    int i = 0;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(cq_->Next(&tag, &ok));
	    GPR_ASSERT(ok);
	    
      std::function<void(void)> func = 
        std::bind(&CallData::Proceed, static_cast<CallData*>(tag));
	    
      TaskProceed task(func);
      pool.submit(task);
      
      std::cout <<"Proceed() done by TID = " << std::this_thread::get_id() <<  std::endl;
      i++;
    }
  }

  std::unique_ptr<ServerCompletionQueue> cq_;
  Greeter::AsyncService service_;
  std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
  ServerImpl server;
  server.Run();

  return 0;
}
