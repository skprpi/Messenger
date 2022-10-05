#ifndef MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR
#define MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR

#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "env.h"
#include "network_navigation.h"
#include "server_config.h"
#include "timeout_limiter.h"

namespace {

class TimeoutLimiterFactory {
private:
    using tcp = boost::asio::ip::tcp;
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(boost::asio::ip::tcp::socket &)>;

public:
    static TimeoutLimiterBuilder create() {
        std::string expiration_raw = Environment::getEnv("SERVER__TIMEOUT_LIMITER__EXPIRATION_MILLISECONDS");

        if (expiration_raw.empty()) {
            return [](tcp::socket &) -> std::shared_ptr<BaseTimeoutLimiter> {
                return std::make_shared<EmptyTimeoutLimiter>();
            };
        }
        uint64_t milliseconds_expiration_ = std::atoi(expiration_raw.c_str());

        std::chrono::milliseconds milliseconds_expiration(milliseconds_expiration_);
        return [milliseconds_expiration](tcp::socket &socket) -> std::shared_ptr<BaseTimeoutLimiter> {
            return std::make_shared<TimeoutLimiter>(socket, milliseconds_expiration);
        };
    }
};

class IPPortFactory {
public:
    static IP_Port create() {
        std::string port_raw = Environment::getEnv("SERVER__PORT");
        std::string ipv4 = Environment::getEnv("SERVER__IPv4");

        assert(!port_raw.empty());
        assert(!ipv4.empty());

        uint16_t port = std::atoi(port_raw.c_str());
        return IP_Port(ipv4, port);
    }
};

}  // namespace

class ServerConfigFactory {
public:
    std::shared_ptr<ServerConfig> static create() {
        return std::make_shared<ServerConfig>(TimeoutLimiterFactory::create(), IPPortFactory::create());
    }
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR
