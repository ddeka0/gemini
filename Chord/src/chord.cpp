#include "common.hpp"
#include "chord.hpp"
/*
What we want to do with this file ?
Create a local Node
make localNode to behave as a grpc server and grpc client

*/

int main() {
    
    std::shared_ptr<LocalNode> node = std::shared_ptr<LocalNode>(new LocalNode());
    std::shared_ptr<GrpcAsyncServer> server = std::shared_ptr<GrpcAsyncServer>(
        new GrpcAsyncServer()
    );
    
    node->addServices();
    std::shared_ptr<Address> addr = std::shared_ptr<Address>(new Address(std::string("5050")));
    node->setAddress(addr);

    node->initialize(server);
    // TODO many things
}