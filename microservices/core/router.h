#ifndef MICROSERVICES_CORE_ROUTER
#define MICROSERVICES_CORE_ROUTER

#include <string>
#include <type_traits>
#include <unordered_map>

#include "base_handler.h"

class Router final {
private:
    void processHandler(std::shared_ptr<BaseHandler> handler) {
        auto inserion_result = handlers.emplace(handler->getName(), handler);
        bool inserted = inserion_result.second;
        if (!inserted) {
            throw "TODO: handler name have to be unique";
        }
    }

public:
    Router(std::shared_ptr<BaseHandler> handler) { processHandler(handler); }

    template <typename... Args>
    Router(std::shared_ptr<BaseHandler> handler, Args &&...other) : Router(std::forward<Args>(other)...) {
        processHandler(handler);
    }

    std::shared_ptr<BaseHandler> getHandler(const Request &request) const {
        // TODO: implement
        return handlers.begin()->second;
    }

    // TODO: Ket type std::invoke_result of getName
    std::unordered_map<uint32_t, std::shared_ptr<BaseHandler>> handlers;
};

#endif  // MICROSERVICES_CORE_ROUTER
