#include "grpcAsyncClient.hpp"
GrpcAsyncClient::GrpcAsyncClient(std::shared_ptr<Channel> channel)
    : stub_(chordMsg::ChordService::NewStub(channel)) {}

GrpcAsyncClient::~GrpcAsyncClient(){}

NodeBase* GrpcAsyncClient::getPredecessor() {
    chordMsg::Empty request;
    chordMsg::NodeAddr reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;
    std::unique_ptr<ClientAsyncResponseReader<chordMsg::NodeAddr>> 
        rpc(stub_->PrepareAsyncgetPredecessor(&context, request, &cq));
    
    rpc->StartCall();
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    RemoteNode* node;
    if (status.ok()) {
        node = new RemoteNode();
        Address addr(reply.ip_addr(),reply.port());
        node->setAddress(addr);
        return node;
    } else {
        return nullptr;
    }
}