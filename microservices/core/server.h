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
    Server(tcp::socket socket, const Router &router, std::shared_ptr<ServerConfig> server_config)
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
        auto self = this->shared_from_this();

        auto request = std::make_shared<Request>();
        auto read_handler = [self, request](beast::error_code ec, std::size_t bytes_transferred) {
            boost::ignore_unused(bytes_transferred);
            if (!ec) self->processRequest(request);
        };
        http::async_read(socket, buffer, *request, read_handler);
    }

    // Determine what needs to be done with the request message.
    void processRequest(std::shared_ptr<Request> request) {
        if (!request) {
            // TODO http error
        }
        std::shared_ptr<BaseHandler> handler = router.getHandler(*request);
        if (!handler) {
            // TODO
        }
        std::shared_ptr<Response> response = handler->getResponce(*request);
        if (!response) {
            // TODO http error
        }
        writeResponceAsync(*response);
    }

    void writeResponceAsync(const Response &response) {
        auto self = this->shared_from_this();
        http::async_write(socket, response, [self](beast::error_code ec, std::size_t) {
            assert(self->timeout_limiter);
            self->socket.shutdown(tcp::socket::shutdown_send, ec);
            self->timeout_limiter->cancel();
        });
    }

    tcp::socket socket;
    beast::flat_buffer buffer{8192};

    const Router &router;
    std::shared_ptr<ServerConfig> server_config;
    std::shared_ptr<BaseTimeoutLimiter> timeout_limiter;
};

#endif  // MICROSERVICES_CORE_SERVER
