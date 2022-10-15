#ifndef MICROSERVICES__CORE__SRC__PRIVATE__NETWORK_NAVIGATION_H
#define MICROSERVICES__CORE__SRC__PRIVATE__NETWORK_NAVIGATION_H

#include <boost/asio.hpp>
#include <string>

class NetworkAddress {
public:
    virtual boost::asio::ip::tcp::endpoint getEndpoint() const = 0;
};

class IP_Port final : public NetworkAddress {
public:
    IP_Port(const std::string& ipv4, const uint16_t port) : ipv4(ipv4), port(port) {}
    IP_Port(const IP_Port& other) : IP_Port(other.ipv4, other.port) {}

    boost::asio::ip::tcp::endpoint getEndpoint() const final {
        return boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ipv4.c_str()), port);
    }

private:
    const std::string ipv4;
    const uint16_t port;
};

#endif  // MICROSERVICES__CORE__SRC__PRIVATE__NETWORK_NAVIGATION_H
