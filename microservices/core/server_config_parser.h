#ifndef MICROSERVICES_CORE_SERVER_CONFIG_PARSER
#define MICROSERVICES_CORE_SERVER_CONFIG_PARSER

#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>

#include <boost/asio.hpp>
#include <fstream>
#include <sstream>

#include "server_config.h"
#include "timeout_limiter.h"

namespace {
using tcp = boost::asio::ip::tcp;

class TimeoutLimiterParser {
private:
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(tcp::socket &)>;

public:
    static TimeoutLimiterBuilder parse(rapidjson::Document &document) {
        if (!document.HasMember("timeout-limiter")) {
            return [](tcp::socket &) -> std::shared_ptr<BaseTimeoutLimiter> {
                return std::make_shared<EmptyTimeoutLimiter>();
            };
        }

        assert(document["timeout-limiter"].HasMember("expiration"));
        uint64_t expiration = document["timeout-limiter"]["expiration"].GetUint64();

        assert(document["timeout-limiter"].HasMember("units"));
        std::string units = document["timeout-limiter"]["units"].GetString();
        assert(units == "second" || units == "millisecond");

        uint64_t millisecond_expiration = (units == "second") ? expiration * 1e6 : expiration;
        return [millisecond_expiration](tcp::socket &socket) -> std::shared_ptr<BaseTimeoutLimiter> {
            return std::make_shared<TimeoutLimiter>(socket, millisecond_expiration);
        };
    }
};

}  // namespace

class ServerConfigParser {
public:
    ServerConfigParser(const std::string &config_path) : config_path(config_path) {}

    std::shared_ptr<ServerConfig> parce() const {
        std::ifstream config_file(config_path);
        assert(config_file.good());

        std::string json((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());

        rapidjson::Document document;
        document.Parse(json.c_str());

        rapidjson::Reader json_reader;
        document.Accept(json_reader);
        assert(document.HasParseError());

        return std::make_shared<ServerConfig>(TimeoutLimiterParser::parse(document));
    }

private:
    std::string config_path;
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG_PARSER
