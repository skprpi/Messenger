#ifndef MICROSERVICES_CORE_SERVER
#define MICROSERVICES_CORE_SERVER

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "alias.h"
#include "base_handler.h"
#include "router.h"
#include "server_config.h"
#include "timeout_limiter.h"

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class Server final : public std::enable_shared_from_this<Server> {
public:
    Server(tcp::socket &&socket, const Router &router, std::shared_ptr<ServerConfig> server_config)
        : socket(std::move(socket)), router(router), server_config(server_config) {}

    // Initiate the asynchronous operations associated with the connection.
    void start() {
        auto builder = server_config->getTimeoutLimiterBuilder();
        timeout_limiter = builder(socket);
        assert(timeout_limiter);
        timeout_limiter->start();

        assert(server_config);
        readRequestAsync();
    }

private:
    void readRequestAsync() {
        auto request = std::make_shared<Request>();
        auto read_handler = [this, request](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) processRequest(request);
        };
        http::async_read(socket, buffer, *request, read_handler);
    }

    void processRequest(std::shared_ptr<Request> request) {
        assert(request);
        std::shared_ptr<BaseHandler> handler = router.getHandler(*request);
        assert(handler);
        std::shared_ptr<Response> response = handler->getResponce(*request);
        assert(response);
        writeResponceAsync(*response);
    }

    void writeResponceAsync(const Response &response) {
        http::async_write(socket, response, [this](beast::error_code ec, std::size_t) {
            assert(timeout_limiter);
            socket.shutdown(tcp::socket::shutdown_send, ec);
            timeout_limiter->cancel();
        });
    }

    tcp::socket socket;
    beast::flat_buffer buffer{8192};

    const Router &router;
    std::shared_ptr<ServerConfig> server_config;
    std::shared_ptr<BaseTimeoutLimiter> timeout_limiter;
};

#endif  // MICROSERVICES_CORE_SERVER
