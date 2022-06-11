#include "grpcAsyncClient.hpp"

GrpcAsyncClient::GrpcAsyncClient(NodeBase* node){
    std::cout <<"GrpcAsyncClient instance created" << std::endl;
    m_node = node;
}

GrpcAsyncClient::~GrpcAsyncClient(){}

void GrpcAsyncClient::notify(NodeBase* remote) {
    chordMsg::NodeAddr request;
    chordMsg::Empty reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;

    // set the request object with 
    request.set_ip_addr(remote->getAddress()->getIpAddr());
    request.set_port(remote->getAddress()->getPort());

    std::string ip_port_addr = m_node->getAddress()->getIpAddr() + ":" + m_node->getAddress()->getPort();
    std::cout <<"notify "<<ip_port_addr << std::endl;

    std::unique_ptr<chordMsg::ChordService::Stub> stub_(
        chordMsg::ChordService::NewStub(grpc::CreateChannel(ip_port_addr, grpc::InsecureChannelCredentials()))
    );

    std::unique_ptr<ClientAsyncResponseReader<chordMsg::Empty>> 
        rpc(stub_->PrepareAsyncNotify(&context, request, &cq));
    
    rpc->StartCall();
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);

    RemoteNode* node;
    if (status.ok()) {
        // node = new RemoteNode();
        // std::string ipAddr = reply.ip_addr();
        // std::string port = reply.port();
        // node->setAddress(
        //     std::shared_ptr<Address>(new Address(ipAddr,port))
        // );
        // return node;
    } else {
        std::cout <<"GrpcAsyncClient::notify failed : "<< status.error_details() << std::endl;
        // return nullptr;
    }    
}

NodeBase* GrpcAsyncClient::getPredecessor() {
    chordMsg::Empty request;
    chordMsg::NodeAddr reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;

    // request nothing to set inside request message
    std::string ip_port_addr = m_node->getAddress()->getIpAddr() + ":" + m_node->getAddress()->getPort();
    std::cout <<"getPredecessor "<< ip_port_addr << std::endl;
    
    std::unique_ptr<chordMsg::ChordService::Stub> stub_(
        chordMsg::ChordService::NewStub(grpc::CreateChannel(ip_port_addr, grpc::InsecureChannelCredentials()))
    );

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
        std::string ipAddr = reply.ip_addr();
        std::string port = reply.port();
        node->setAddress(
            std::shared_ptr<Address>(new Address(ipAddr,port))
        );

		std::shared_ptr<GrpcAsyncClient> client = std::shared_ptr<GrpcAsyncClient>(
			new GrpcAsyncClient(node)
		);  

		node->initWithClient(client);
        return node;
    } else {
        std::cout <<"GrpcAsyncClient::getPredecessor failed : "<<status.error_details() << std::endl;
        return nullptr;
    }
}

NodeBase* GrpcAsyncClient::findSuccessor(unsigned int Id) {
    chordMsg::Id request;
    chordMsg::NodeAddr reply;
    ClientContext context;
    grpc::CompletionQueue cq;
    grpc::Status status;

    request.set_id(Id);
    // auto stub_ = setupNewChannel(); // create a new channel
    if(this == nullptr) {
        std::cout<<"**************"<<std::endl;
    }
    std::cout <<"this = " << this << " m_node = "<< m_node <<std::endl;
    std::string ip_port_addr = m_node->getAddress()->getIpAddr() + ":" + m_node->getAddress()->getPort();
    std::cout <<"findSuccessor : "<<ip_port_addr << std::endl;

    std::unique_ptr<chordMsg::ChordService::Stub> stub_(chordMsg::ChordService::NewStub(
        grpc::CreateChannel(ip_port_addr, grpc::InsecureChannelCredentials()))
    );

    std::unique_ptr<ClientAsyncResponseReader<chordMsg::NodeAddr>> rpc(
        stub_->PrepareAsyncFindSuccessor(&context, request, &cq)
    );
    
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
        std::string ipAddr = reply.ip_addr();
        std::string port = reply.port();
        std::cout<<"Reply from remoteNode : ip = "<<ipAddr<<" and port = "<<port<<std::endl;
        node->setAddress(
            std::shared_ptr<Address>(new Address(ipAddr,port))
        );
		std::shared_ptr<GrpcAsyncClient> client = std::shared_ptr<GrpcAsyncClient>(
			new GrpcAsyncClient(node)
		);  

		node->initWithClient(client);
        return node;
    } else {
        std::cout <<"GrpcAsyncClient::findSuccessor failed : "<<status.error_details() << std::endl;
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