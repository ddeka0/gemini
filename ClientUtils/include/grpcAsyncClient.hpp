#pragma once
#include "Chord/include/common.hpp"
#include "Node/include/remoteNode.hpp"


#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "protocol/messages.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

class GrpcAsyncClient {
public:
    explicit GrpcAsyncClient(std::shared_ptr<Channel> channel);
    virtual ~GrpcAsyncClient();
    NodeBase* getPredecessor();
    
private:
	
	std::unique_ptr<chordMsg::ChordService::Stub> stub_;
}



