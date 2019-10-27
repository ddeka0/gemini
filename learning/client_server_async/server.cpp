/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <bits/stdc++.h>
#include <variant>
#include <time.h>
#include <sys/time.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
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

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;


/*============== Thread Pool code starts ============== */
#define THREAD_POOL_SIZE 4
std::map<thread::id,std::string> tidToTname;

void delayMillis(int n) { /* used in test purpose */
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = n * 1000000L;
    nanosleep(&req, (struct timespec *)NULL);
}
void delayNanos(int n) { /* used in test purpose */
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = n;
    nanosleep(&req, (struct timespec *)NULL);
}
template<typename T>
class threadSafeQueue {
    private:
        mutable std::mutex mut;
        std::queue<std::shared_ptr<T>> taskQueue; /* 	task as pushed here and 
							task are processed in FIFO
							style */
        std::condition_variable dataCond;		/* used to protect the queue */
    public:
        threadSafeQueue(){}
        void waitAndPop(T& value);		/* wait untill task is not available in 
										   the queue */
        std::shared_ptr<T> waitAndPop();/* same but returns a shared pointer */
        bool tryPop(T& value);			/* does not block */
        std::shared_ptr<T> tryPop();    /* does not block and returns a pointer*/
        void Push(T newData);
        bool IsEmpty() const;				/* check if queue is empty or not */
		void notifyAllThreads();		/* notify all the waiting threads
										   used in Thpool decallocation	*/
        int Size();
};

template<typename T>
void threadSafeQueue<T>::notifyAllThreads() {
    dataCond.notify_all();
}
template<typename T>
void threadSafeQueue<T>::waitAndPop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    dataCond.wait(lk,[this](){return !taskQueue.empty();});
    value = std::move(*taskQueue.front());
    taskQueue.pop();
}
template<typename T>
std::shared_ptr<T> threadSafeQueue<T>::waitAndPop() {
    std::unique_lock<std::mutex> lk(mut);
    dataCond.wait(lk,[this](){return !taskQueue.empty();});
    std::shared_ptr<T> res = taskQueue.front();
    taskQueue.pop();
    return res;
}

template<typename T>
bool threadSafeQueue<T>::tryPop(T& value) {
    std::lock_guard<std::mutex> lk(mut);
    if(taskQueue.empty())
        return false;
    value = std::move(*taskQueue.front());
    taskQueue.pop();
    return true;
}   
template<typename T>
std::shared_ptr<T> threadSafeQueue<T>::tryPop() {
    std::lock_guard<std::mutex> lk(mut);
    if(taskQueue.empty())
        return std::shared_ptr<T>(); /* return nullptr */
    std::shared_ptr<T> res = taskQueue.front();
    taskQueue.pop();
    return res;
}
template<typename T>
void threadSafeQueue<T>::Push(T newData) { /* TODO: size check before pushing */
    // std::shared_ptr<T> data(std::make_shared<T>(std::move(newData)));
    auto data = std::make_shared<T>(std::move(newData));
										   /* construct the object before lock*/
    std::lock_guard<std::mutex> lk(mut);
    taskQueue.push(std::move(data));
    dataCond.notify_one();
}
template<typename T>
bool threadSafeQueue<T>::IsEmpty() const {
    std::lock_guard<std::mutex> lk(mut);
    return taskQueue.empty();
}

template<typename T>
int threadSafeQueue<T>::Size() {
    std::lock_guard<std::mutex> lk(mut);
    return taskQueue.size();
}

class threadJoiner { /* could be used to automatic thread deallocation 
						not used for now, TODO use it in the exception case */
    private:
        std::vector<std::thread> & threads;
    public:
        explicit threadJoiner(std::vector<std::thread>& thread_):threads(thread_){}
        ~threadJoiner() {
            for(unsigned long i = 0;i<threads.size();i++) {
                if(threads[i].joinable()) {
                    threads[i].join();
                }
            }
        }
};

class TaskRPCB { /* Task RecvAndProcessCallbacks */
    private:
    	int len;
		uint fdId;
        int streamId;
		void* msgBlob;
		std::function<void(void*,int,uint,int)> func;
    public:
        TaskRPCB(std::function<void(void*,int,uint,int)>&f , void* msgBlob,int len,
			uint fdId, int streamId) {
            this->func = f;
			this->msgBlob = msgBlob;
        	this->len = len;
			this->fdId = fdId;
            this->streamId = streamId;
        }
        void operator()() {
            func(msgBlob,len,fdId,streamId);
        }
};

class TaskECB { /* Task eventCB */
    private:
		std::function<void(void)> func;
    public:
        TaskECB(std::function<void(void)>&f) : func(f) {}
        void operator()() {
            func();
        }
};

class TaskTimer { /* Task timeEvent */
    private:
		std::function<void(void)> func;
    public:
        TaskTimer(std::function<void(void)>&f) : func(f) {}
        void operator()() {
            func();
        }
};


class TaskProceed {
	private:
		std::function<void(void)> func;
	public:
		TaskProceed(std::function<void(void)>&f) : func(f) {}
		void operator()() {
			func();
		}
};
using TaskSet = std::variant<TaskECB,TaskRPCB,TaskTimer,TaskProceed>;
struct executeTask { // TODO : replace this visit+lambda pattern later
	void operator()(TaskECB & task) { 
		task();
	}
	void operator()(TaskRPCB& task) {
		task();
	}
	void operator()(TaskTimer& task) {
		task();
	}
	void operator()(TaskProceed& task) {
		task();
	}
};

class Thpool {
    std::atomic_bool done;
    threadSafeQueue<TaskSet> workQ;
    std::vector<std::thread> threads;
    void workerThread() {
        while(!done) {
            auto task = workQ.waitAndPop();
            if(task != nullptr and !done) {
                std::visit(executeTask(), (*task));
            }
        }
    }
    public:
        Thpool(): done(false) /* ,joiner(threads) */ {
	        unsigned const maxThreadCount = THREAD_POOL_SIZE;
	        printf("ThreadPool Size = %u\n",maxThreadCount);
            try {
                for(unsigned int i = 0;i<maxThreadCount;i++) {
                    threads.push_back(std::thread(&Thpool::workerThread,this));
                }
            }catch(...) {
                done = true;
                throw;
            }
        }
        ~Thpool() {
            // done = true;
        }
        template<typename taskType>
        void submit(taskType task) {
            workQ.Push(task);
        }
        void deAllocatePool() {
            done = true;
            workQ.notifyAllThreads();
            //unsigned const maxThreadCount = std::thread::hardware_concurrency();
            unsigned const maxThreadCount = THREAD_POOL_SIZE;
	    	for(unsigned int i = 0;i<maxThreadCount;) {
                if(threads[i].joinable()) {
                    threads[i].join();
                    i++; /* go for the next thread in the pool */
                }else {
					/*did not wake up, do it again */
                	workQ.notifyAllThreads();
                }
            }
        }

        int checkSize() {
            return workQ.Size();
        }
};
/*============== Thread Pool code ends ============== */










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
