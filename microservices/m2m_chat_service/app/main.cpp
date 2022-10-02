#include <boost/asio.hpp>
#include <iostream>

#include "core/router.h"
#include "core/server.h"
#include "get.h"

enum class HandlerType : uint32_t {
    GET = 0,
};

int main() {
    auto get_handler = std::make_shared<m2m_chat_service::GetHandler>(HandlerType::GET);
    Router router(get_handler);

    startServer(router);
    return 0;
}
