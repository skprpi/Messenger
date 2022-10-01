#ifndef MICROSERVICES_CORE_SERVER_CONFIG
#define MICROSERVICES_CORE_SERVER_CONFIG

#include "server_config_parser.h"
#include "timeout_limiter.h"

class ServerConfig {
private:
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(tcp::socket &)>;

public:
    ServerConfig(const TimeoutLimiterBuilder &timeout_limiter_builder)
        : timeout_limiter_builder{timeout_limiter_builder} {}

    TimeoutLimiterBuilder getTimeoutLimiterBuilder() const { return timeout_limiter_builder; }

private:
    TimeoutLimiterBuilder timeout_limiter_builder;
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG
