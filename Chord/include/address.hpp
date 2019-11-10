#pragma once
#include "common.hpp"
class Address {
public:
    Address() {
        // default constructor
    } 
    ~Address() {
        // default destructor
    }
    Address(std::string &ipAddr,std::string &port) 
        : m_ipv4Addr(ipAddr),m_port(port) {
        // add the checking of ipv4 addr or ipv6 addr
    }
    Address(std::string &port,std::string ipAddr = "localhost") 
        : m_port(port){
        
    }

protected:
private:
    std::string m_ipv4Addr;
    std::string m_ipv6Addr;
    std::string m_port;
};