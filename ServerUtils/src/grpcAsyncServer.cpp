#include "grpcAsyncServer.hpp"
#include "Chord/include/address.hpp"
GrpcAsyncServer::GrpcAsyncServer(){};
GrpcAsyncServer::~GrpcAsyncServer(){};
void GrpcAsyncServer::Run() {
	// place you code
	// std::string server_address("0.0.0.0:50051");
	// ServerBuilder builder;
	// builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// builder.RegisterService(&service_);
	// cq_ = builder.AddCompletionQueue();
	// server_ = builder.BuildAndStart();
	// std::cout << "Server listening on " << server_address << std::endl;
	// HandleRpcs();
}

void GrpcAsyncServer::HandleRpcs() {
	
	// CallData data{&service_,cq_.get()};
	// new AddCall(&data);
	// new SubCall(&data);
	// void* tag;
	// bool ok;
	// while (true) {
	// 	GPR_ASSERT(cq_->Next(&tag, &ok));
	// 	GPR_ASSERT(ok);
	// 	if(tag == nullptr) {
	// 		cout <<"someting wrong !"<<endl;
	// 	}else {	
	// 		static_cast<Call*>(tag)->Proceed();
	// 	}
	// }
}