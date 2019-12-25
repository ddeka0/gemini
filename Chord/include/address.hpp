#pragma once
#include "common.hpp"
class Address {
public:
    Address();
    virtual ~Address();
    Address(std::string &ipAddr,std::string &port);

    Address(std::string &port,std::string ipAddr = "localhost");
    // had to overload this, because of rvalue reference
    Address(std::string &&ipAddr,std::string &&port);
    Address(std::string &&port,std::string ipAddr = "localhost");
    std::string toString();
protected:
private:
    std::string m_ipv4Addr;
    std::string m_ipv6Addr;
    std::string m_port;
};