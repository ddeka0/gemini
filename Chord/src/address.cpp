#pragma once
#include "common.hpp"
#include "address.hpp"

Address::Address(){}
Address::~Address(){}

Address::Address(std::string &ipAddr,std::string &port) 
    : m_ipv4Addr(ipAddr),m_port(port) {
    // add the checking of ipv4 addr or ipv6 addr
}

// Address::Address(std::string &port,std::string ipAddr) 
//     : m_port(port),m_ipv4Addr(ipAddr) {   
// }

// had to overload this, because of rvalue reference
Address::Address(std::string &&ipAddr,std::string &&port) 
    : m_ipv4Addr(ipAddr),m_port(port) {
    // add the checking of ipv4 addr or ipv6 addr
}
// Address::Address(std::string &&port,std::string ipAddr) 
//     : m_port(port),m_ipv4Addr(ipAddr){
    
// }
std::string Address::toString() {
    return std::string(m_ipv4Addr + ":" + m_port);
}

std::string Address::getIpAddr() {
    return m_ipv4Addr;
}

std::string Address::getPort() {
    return m_port;
}