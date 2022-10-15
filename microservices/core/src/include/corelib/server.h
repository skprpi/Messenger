#ifndef MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__SERVER_H
#define MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__SERVER_H

#include <boost/asio.hpp>
#include <memory>

#include "alias.h"
#include "http_assert.h"
#include "router.h"
#include "../../private/server_config.h"
#include "../../private/server_config_factory.h"
#include "../../private/timeout_limiter.h"
#include "../../private/http_connection.h"


void startServer(const Router &router) {
    boost::asio::io_context io_context;

    std::shared_ptr<ServerConfig> config = ServerConfigFactory::create();
    LOGIC_ASSERT(config, "Got nullptr config");

    boost::asio::ip::tcp::acceptor acceptor(io_context, config->getEndpoint());

    startServerLoop(acceptor, io_context, router, config);
    std::cout << "service started!\n";

    io_context.run();
}

#endif  // MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__SERVER_H
