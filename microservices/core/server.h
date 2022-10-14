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
#include "http_assert.h"
#include "router.h"
#include "server_config.h"
#include "server_config_factory.h"
#include "timeout_limiter.h"

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

class HTTPConnection final : public std::enable_shared_from_this<HTTPConnection> {
public:
    HTTPConnection(
            boost::asio::io_context &io_context, const Router &router, std::shared_ptr<ServerConfig> server_config)
        : socket(io_context), router(router), server_config(server_config) {}

    // Initiate the asynchronous operations associated with the connection.
    void start() {
        auto builder = server_config->getTimeoutLimiterBuilder();
        timeout_limiter = builder(socket);
        LOGIC_ASSERT(timeout_limiter, "Got nullptr timeout_limiter");
        timeout_limiter->start();
        LOGIC_ASSERT(server_config, "Got nullptr server_config");
        readRequestAsync();
    }

    tcp::socket &getSocket() { return socket; }

private:
    void readRequestAsync() {
        auto request = std::make_shared<Request>();
        auto read_handler = [self = shared_from_this(), request](beast::error_code ec, std::size_t bytes_transferred) {
            // capture request for async_read also
            boost::ignore_unused(bytes_transferred);
            if (!ec) {
                self->processRequest(request);
            }
        };
        LOGIC_ASSERT(request, "Got nullptr request");
        http::async_read(socket, buffer, *request, read_handler);
    }

    void processRequest(std::shared_ptr<Request> request) {
        LOGIC_ASSERT(request, "Got nullptr request");
        std::shared_ptr<Response> response = router.getResponse(request);
        LOGIC_ASSERT(response, "Got nullptr response");
        writeResponceAsync(response);
    }

    void writeResponceAsync(std::shared_ptr<Response> response) {
        LOGIC_ASSERT(response, "Got nullptr response");
        response->content_length(response->body().size());
        http::async_write(socket, *response, [self = shared_from_this(), response](beast::error_code ec, std::size_t) {
            // we have to capture response to use it via async_write, instead it will be segmentation fault
            std::cout << "message has been written\n";
            LOGIC_ASSERT(self->timeout_limiter, "Got nullptr timeout_limiter");
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

// Forever async loop
void startServerLoop(tcp::acceptor &acceptor, boost::asio::io_context &io_context, const Router &router,
        std::shared_ptr<ServerConfig> server_config) {
    LOGIC_ASSERT(server_config, "Got nullptr server_config");
    auto http_connection = std::make_shared<HTTPConnection>(io_context, router, server_config);
    auto &socket = http_connection->getSocket();

    acceptor.async_accept(
            socket, [&acceptor, &router, &io_context, server_config, http_connection](beast::error_code ec) {
                if (!ec) {
                    http_connection->start();
                }
                startServerLoop(acceptor, io_context, router, server_config);
            });
}

void startServer(const Router &router) {
    boost::asio::io_context io_context;

    std::shared_ptr<ServerConfig> config = ServerConfigFactory::create();
    LOGIC_ASSERT(config, "Got nullptr config");

    boost::asio::ip::tcp::acceptor acceptor(io_context, config->getEndpoint());

    startServerLoop(acceptor, io_context, router, config);
    std::cout << "service started!\n";

    io_context.run();
}

#endif  // MICROSERVICES_CORE_SERVER
