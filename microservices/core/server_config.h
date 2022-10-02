#ifndef MICROSERVICES_CORE_SERVER_CONFIG
#define MICROSERVICES_CORE_SERVER_CONFIG

#include "timeout_limiter.h"
#include "network_navigation.h"
#include <boost/asio.hpp>

class ServerConfig {
private:
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(boost::asio::ip::tcp::socket&)>;

public:
    ServerConfig(const TimeoutLimiterBuilder &timeout_limiter_builder, IP_Port ip_port)
        : timeout_limiter_builder(timeout_limiter_builder), ip_port(ip_port) {}

    TimeoutLimiterBuilder getTimeoutLimiterBuilder() const { return timeout_limiter_builder; }

    boost::asio::ip::tcp::endpoint getEndpoint() const {
        return ip_port.getEndpoint();
    }

private:
    TimeoutLimiterBuilder timeout_limiter_builder;
    IP_Port ip_port; // provide endpoint
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG
