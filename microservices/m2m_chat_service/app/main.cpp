#include <boost/asio.hpp>
#include <filesystem>
#include <iostream>

#include "core/router.h"
#include "core/server.h"
#include "core/server_config_parser.h"
#include "get.h"

enum class HandlerType : uint32_t {
    GET = 0,
};

int main(int argc, char** argv) {
    assert(argc == 2);
    std::filesystem::path server_config_path(
        std::string(std::filesystem::current_path()).append(argv[1]));  // .append("server_config.json")
    ServerConfigParser parser(server_config_path);

    boost::asio::io_context io_context;
    // TODO: fix make_adress

    // boost::asio::ip::tcp::acceptor acceptor(io_context, {boost::asio::ip::make_address("e3"), 80});
    boost::asio::ip::tcp::socket socket(io_context);

    HandlerType type(HandlerType::GET);
    auto get_handler = std::make_shared<m2m_chat_service::GetHandler>(type);
    Router router(get_handler);

    auto config = parser.parce();
    Server server(std::move(socket), router, config);
    server.start();
    std::cout << "m2m_chat_service started!\n";

    io_context.run();
    return 0;
}
