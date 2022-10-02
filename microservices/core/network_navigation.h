#ifndef MICROSERVICES_CORE_NETWORK_NAVIGATION
#define MICROSERVICES_CORE_NETWORK_NAVIGATION

#include <string>
#include <boost/asio.hpp>

struct IP_Port {
    IP_Port(const std::string& ipv4, const uint16_t port) : ipv4(ipv4), port(port) {}
    IP_Port(const IP_Port &other) : IP_Port(other.ipv4, other.port) {}

    boost::asio::ip::tcp::endpoint getEndpoint() const {
        return boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ipv4.c_str()), port);
    }

    const std::string ipv4;
    const uint16_t port;
};

#endif // MICROSERVICES_CORE_NETWORK_NAVIGATION
