#ifndef MICROSERVICES_M2M_CHAT_SERVICE_APP_GET
#define MICROSERVICES_M2M_CHAT_SERVICE_APP_GET

#include <boost/beast/http.hpp>

#include "core/base_handler.h"

namespace m2m_chat_service {

class GetHandler : public BaseHandler {
public:
    template <typename HandlerNameType>
    GetHandler(HandlerNameType name) : BaseHandler(name) {}

    std::shared_ptr<Response> getResponce(const Request &request) const {
        Response response;
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "text/html");
        response.body() = "name=foo";
        return std::make_shared<Response>(response);
    }
};

}  // namespace m2m_chat_service

#endif  // MICROSERVICES_M2M_CHAT_SERVICE_APP_GET
