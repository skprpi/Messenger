#ifndef MICROSERVICES_CORE_BASE_HANDLER
#define MICROSERVICES_CORE_BASE_HANDLER

#include <type_traits>

#include "alias.h"

class BaseHandler {
public:
    template <typename HandlerNameType>
    BaseHandler(HandlerNameType enum_name) {
        name = static_cast<uint32_t>(enum_name);
        static_assert(std::is_enum<HandlerNameType>::value, "Type HandlerNameType have to be based on the enum class");
    }

    virtual std::shared_ptr<Response> getResponce(const Request &request) const = 0;

    uint32_t getName() const { return name; }

    virtual ~BaseHandler() {}

private:
    uint32_t name;
};

#endif  // MICROSERVICES_CORE_BASE_HANDLER
