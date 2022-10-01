#ifndef MICROSERVICES_M2M_CHAT_SERVICE_APP_GET
#define MICROSERVICES_M2M_CHAT_SERVICE_APP_GET

#include "core/base_handler.h"

namespace m2m_chat_service {

class GetHandler : public BaseHandler {
public:
    template <typename HandlerNameType>
    GetHandler(HandlerNameType name) : BaseHandler(name) {}

    std::shared_ptr<Response> getResponce(const Request &request) const {
        Response response;
        return std::make_shared<Response>(response);
    }
};

}  // namespace m2m_chat_service

#endif  // MICROSERVICES_M2M_CHAT_SERVICE_APP_GET
