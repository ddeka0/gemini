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

using ClientStub = chordMsg::ChordService::Stub;

class GrpcAsyncClient {
public:
    explicit GrpcAsyncClient(NodeBase*);
    virtual ~GrpcAsyncClient();
    NodeBase* getPredecessor();
    NodeBase* findSuccessor(unsigned int Id);
    static ClientStub* setupNewChannel();
    
protected:
private:
    NodeBase* m_node;
    
};