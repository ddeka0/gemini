#include "common.hpp"
#include "chord.hpp"
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {
    auto result = parse(argc, argv);
    // Create a LocalNode instance
    std::shared_ptr<LocalNode> node = std::shared_ptr<LocalNode>(new LocalNode());
    
    /* Create a server object and pass the node pointer to it
    */
    std::shared_ptr<GrpcAsyncServer> server = std::shared_ptr<GrpcAsyncServer>(
        new GrpcAsyncServer(node.get())
    );
    
    node->addServices(GET_SERVICE_FLAG);

    // set the IP+PORT of the node, which will be used by the server object
    std::string local_port = result["local-port"].as<std::string>();
    std::shared_ptr<Address> addr = std::shared_ptr<Address>(new Address(local_port));
    
    node->setAddress(addr);

    // set the server
    node->initialize(server);

    std::thread t1(&LocalNode::stabilize,node);


    node->Start();
    t1.join();
    
}