#ifndef MICROSERVICES_CORE_BASE_HANDLER
#define MICROSERVICES_CORE_BASE_HANDLER

#include <type_traits>

#include "alias.h"

class BaseHandler {
public:
    template <typename HandlerNameType>
    BaseHandler(HandlerNameType name) : name(name) {
        static_assert(std::is_enum<HandlerNameType>::value, "Type HandlerNameType have to be based on the enum class");
        static_assert(std::is_same<typename std::underlying_type<HandlerNameType>, uint32_t>::value,
                      "Enum have to be base on uint32_t");
    }

    virtual std::shared_ptr<Response> getResponce(const Request &request) const = 0;

    uint32_t getName() const { return name; }

    virtual ~BaseHandler() {}

private:
    uint32_t name;
};

#endif  // MICROSERVICES_CORE_BASE_HANDLER
