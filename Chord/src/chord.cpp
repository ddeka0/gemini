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

    // Add try catch block for these argument parsing lines
    std::string local_port = result["local-port"].as<std::string>();
    std::string local_ip = result["local-ip"].as<std::string>();
    std::shared_ptr<Address> local_addr = std::shared_ptr<Address>(new Address(local_ip,local_port));
    node->setAddress(local_addr);
    
    // Add try catch block for these argument parsing lines
    std::string remote_port = result["remote-port"].as<std::string>();
    std::string remote_ip = result["remote-ip"].as<std::string>();
    auto remote_addr = std::shared_ptr<Address>(new Address(remote_ip,remote_port));
    node->join(remote_addr.get()); // just passing a normal pointer

    // set the server
    node->initialize(server);

    std::thread t1(&LocalNode::stabilize,node);
    std::thread t2(&LocalNode::fixFingers,node);

    node->Start();
    t1.join();
    t2.join();
    
}