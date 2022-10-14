#ifndef MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR
#define MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR

#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "env.h"
#include "http_assert.h"
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

class NetworkAddressFactory {
public:
    static std::shared_ptr<NetworkAddress> create() {
        std::string port = Environment::getEnv("SERVER__PORT");
        std::string address = Environment::getEnv("SERVER__ADDRESS");

        ENV_VARIABLE_ASSERT(!port.empty(), "SERVER__PORT is empty");
        ENV_VARIABLE_ASSERT(!address.empty(), "SERVER__ADDRESS is empty");

        if (true/* TODO: add checks that address is ipv4 */) {
            const uint16_t port_ = std::stoi(port);
            return std::make_shared<IP_Port>(address, port_);
        }
        throw EnvironmentVariableException("Type of address undentified");
    }
};

}  // namespace

class ServerConfigFactory {
public:
    std::shared_ptr<ServerConfig> static create() {
        return std::make_shared<ServerConfig>(TimeoutLimiterFactory::create(), NetworkAddressFactory::create());
    }
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG_VALIDATOR
