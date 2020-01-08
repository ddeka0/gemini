#include "grpcAsyncClient.hpp"

GrpcAsyncClient::GrpcAsyncClient(NodeBase* node){
    std::cout <<"GrpcAsyncClient instance created" << std::endl;
    m_node = node;
}

GrpcAsyncClient::~GrpcAsyncClient(){}

NodeBase* GrpcAsyncClient::getPredecessor() {
    chordMsg::Empty request;
    chordMsg::NodeAddr reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;

    std::unique_ptr<chordMsg::ChordService::Stub> stub_(chordMsg::ChordService::NewStub(grpc::CreateChannel("localhost:6767", grpc::InsecureChannelCredentials())));

    std::unique_ptr<ClientAsyncResponseReader<chordMsg::NodeAddr>> 
        rpc(stub_->PrepareAsyncgetPredecessor(&context, request, &cq));
    
    rpc->StartCall();
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    std::cout<<"waiting for getPredecessor() response" << std::endl;
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    RemoteNode* node;
    if (status.ok()) {
        node = new RemoteNode();
        std::string ipAddr = reply.ip_addr();
        std::string port = reply.port();
        node->setAddress(
            std::shared_ptr<Address>(new Address(ipAddr,port))
        );
        return node;
    } else {
        return nullptr;
    }
}

NodeBase* GrpcAsyncClient::findSuccessor(unsigned int Id) {
    chordMsg::Id request;
    chordMsg::NodeAddr reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;

    // auto stub_ = setupNewChannel(); // create a new channel
    std::unique_ptr<chordMsg::ChordService::Stub> stub_(chordMsg::ChordService::NewStub(grpc::CreateChannel("localhost:6767", grpc::InsecureChannelCredentials())));

    std::unique_ptr<ClientAsyncResponseReader<chordMsg::NodeAddr>> rpc(stub_->PrepareAsyncFindSuccessor(&context, request, &cq));
    
    rpc->StartCall();
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    std::cout<<"waiting for findSuccessor() response" << std::endl;
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    RemoteNode* node;
    if (status.ok()) {
        node = new RemoteNode();
        std::string ipAddr = reply.ip_addr();
        std::string port = reply.port();
        std::cout<<"Reply from remoteNode : ip = "<<ipAddr<<" and port = "<<port<<std::endl;
        node->setAddress(
            std::shared_ptr<Address>(new Address(ipAddr,port))
        );
        return node;
    } else {
        return nullptr;
    } 
}



ClientStub* GrpcAsyncClient::setupNewChannel() {
    // auto stub_ = std::unique_ptr<chordMsg::ChordService::Stub>(
    //     chordMsg::ChordService::NewStub(
    //         grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())
    //     )
    // );
    std::unique_ptr<chordMsg::ChordService::Stub> stub_(chordMsg::ChordService::NewStub(grpc::CreateChannel("localhost:6767", grpc::InsecureChannelCredentials())));
    return stub_.get();
}