#ifndef MICROSERVICES_CORE_SERVER_CONFIG_PARSER
#define MICROSERVICES_CORE_SERVER_CONFIG_PARSER

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "server_config.h"
#include "timeout_limiter.h"

namespace {

class TimeoutLimiterParser {
private:
    using tcp = boost::asio::ip::tcp;
    using TimeoutLimiterBuilder = std::function<std::shared_ptr<BaseTimeoutLimiter>(boost::asio::ip::tcp::socket &)>;

public:
    static TimeoutLimiterBuilder parse(rapidjson::Document &document) {
        if (!document.HasMember("timeout-limiter")) {
            return [](tcp::socket &) -> std::shared_ptr<BaseTimeoutLimiter> {
                return std::make_shared<EmptyTimeoutLimiter>();
            };
        }

        assert(document["timeout-limiter"].HasMember("expiration"));
        uint64_t expiration = document["timeout-limiter"]["expiration"].GetUint64();

        assert(document["timeout-limiter"].HasMember("expiration-units"));
        std::string expiration_units = document["timeout-limiter"]["expiration-units"].GetString();
        assert(expiration_units == "second" || expiration_units == "millisecond");

        expiration = (expiration_units == "second") ? expiration * 1e6 : expiration;
        std::chrono::milliseconds millisecond_expiration(expiration);
        return [millisecond_expiration](tcp::socket &socket) -> std::shared_ptr<BaseTimeoutLimiter> {
            return std::make_shared<TimeoutLimiter>(socket, millisecond_expiration);
        };
    }
};

}  // namespace

class ServerConfigParser {
public:
    ServerConfigParser(const std::filesystem::path &config_path) : config_path(config_path) {}

    std::shared_ptr<ServerConfig> parce() const {
        std::ifstream config_file(std::filesystem::absolute(config_path));
        assert(config_file.good());
        rapidjson::IStreamWrapper stream_wrapper(config_file);

        rapidjson::Document document;
        document.ParseStream(stream_wrapper);

        // TODO change to rapidjson::Reader
        // rapidjson::StringBuffer buffer {};
        // rapidjson::Writer<rapidjson::StringBuffer> json_reader(buffer);
        // document.Accept(json_reader);
        assert(!document.HasParseError());

        return std::make_shared<ServerConfig>(TimeoutLimiterParser::parse(document));
    }

private:
    std::filesystem::path config_path;
};

#endif  // MICROSERVICES_CORE_SERVER_CONFIG_PARSER
