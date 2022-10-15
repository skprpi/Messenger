#ifndef MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__HTTP_ASSERT_H
#define MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__HTTP_ASSERT_H

#include <exception>
#include <string>

#define CREATE_HTTP_EXCEPTION_AND_ASSERTATION(exception_class_name, assertation_name) \
    struct exception_class_name : public std::exception {                             \
        exception_class_name(std::string&& message) : message(std::move(message)) {}  \
        virtual const char* what() const noexcept { return message.c_str(); }         \
                                                                                      \
    private:                                                                          \
        const std::string message;                                                    \
    };                                                                                \
    template <typename T>                                                             \
    void assertation_name(T&& condition, std::string&& message) {                     \
        if (static_cast<bool>(condition)) {                                           \
            return;                                                                   \
        }                                                                             \
        throw exception_class_name(std::move(message));                               \
    }

CREATE_HTTP_EXCEPTION_AND_ASSERTATION(LogicException, LOGIC_ASSERT)
CREATE_HTTP_EXCEPTION_AND_ASSERTATION(BadRequestException, BAD_REQUEST_ASSERT)
CREATE_HTTP_EXCEPTION_AND_ASSERTATION(ParseException, PARSE_ASSERT)
CREATE_HTTP_EXCEPTION_AND_ASSERTATION(EnvironmentVariableException, ENV_VARIABLE_ASSERT)

#undef CREATE_HTTP_EXCEPTION_AND_ASSERTATION

#endif  // MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__HTTP_ASSERT_H
