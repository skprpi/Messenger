#include <boost/asio.hpp>
#include <iostream>

#include "core/router.h"
#include "core/server.h"

std::shared_ptr<Response> fake_func(CallbackInfo&&) {
    std::cout << "Fake func!" << std::endl;
    return std::make_shared<Response>();
}

int main() {
    Router router;
    router.handleFunc("/{123}/path", "GET", &fake_func);
    // router.handleFunc("/{id}/path", "GET", &fake_func);

    startServer(router);
    return 0;
}
