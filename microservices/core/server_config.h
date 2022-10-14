#ifndef MICROSERVICES_CORE_SERVER_CONFIG
#define MICROSERVICES_CORE_SERVER_CONFIG

#include <boost/asio.hpp>

#include "network_navigation.h"
#include "timeout_limiter.h"
#include "http_assert.h"

class ServerConfig {
private:
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(boost::asio::ip::tcp::socket &)>;

public:
    ServerConfig(const TimeoutLimiterBuilder &timeout_limiter_builder, std::shared_ptr<NetworkAddress> address)
        : timeout_limiter_builder(timeout_limiter_builder), address(address) {}

    TimeoutLimiterBuilder getTimeoutLimiterBuilder() const { return timeout_limiter_builder; }

    boost::asio::ip::tcp::endpoint getEndpoint() const {
        LOGIC_ASSERT(address, "Got nullptr address");
        return address->getEndpoint();
    }

private:
    TimeoutLimiterBuilder timeout_limiter_builder;
    std::shared_ptr<NetworkAddress> address;  // provide endpoint
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG
