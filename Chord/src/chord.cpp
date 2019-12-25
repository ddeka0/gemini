#include "common.hpp"
#include "chord.hpp"

int main() {
    // Create a LocalNode instance
    std::shared_ptr<LocalNode> node = std::shared_ptr<LocalNode>(new LocalNode());
    
    // Create a Server object
    std::shared_ptr<GrpcAsyncServer> server = std::shared_ptr<GrpcAsyncServer>(
        new GrpcAsyncServer(node.get())
    );
    // add services to the node
    std::cout <<"h1" << std::endl;
    
    node->addServices();

    // set the IP+PORT of the node, which will be used by the server object
    std::shared_ptr<Address> addr = std::shared_ptr<Address>(new Address(std::string("5050")));
    node->setAddress(addr);

    // set the server
    node->initialize(server);
    std::cout <<"h2" << std::endl;

    node->Start();
}