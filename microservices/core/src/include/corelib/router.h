#ifndef MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ROUTER_H
#define MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ROUTER_H

#include <deque>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include "../../private/target_resolver.h"
#include "alias.h"

struct CallbackInfo;

class Router final {
public:
    Router() = default;

    void handleFunc(std::string &&url, std::string &&http_method, EndpointFunction callback) {
        resolver.addEndpoint(std::move(url), std::move(http_method), callback);
    }

    std::shared_ptr<Response> getResponse(std::shared_ptr<Request> request) const {
        std::string http_method = request->method_string().to_string();
        std::string url = request->base().target().to_string();

        try {
            // get
            auto function = resolver.getCallback(url, http_method);
            return function();
        } catch (BadRequestException& ex) {
            std::cout << "bad function: " << ex.what() << std::endl;
            auto response = std::make_shared<Response>();
            response->body() = "bad response";
            return response;
        }
    }

private:
    // TODO: Ket type std::invoke_result of getName
    TargetResolver resolver;
};

#endif  // MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ROUTER_H
