#ifndef MICROSERVICES__CORE__SRC__PRIVATE__ENV_H
#define MICROSERVICES__CORE__SRC__PRIVATE__ENV_H

#include <iostream>
#include <string>
#include <string_view>

class Environment {
public:
    static std::string getEnv(const std::string_view env_variable) {
        char* res = std::getenv(env_variable.data());
        if (res == nullptr) {
            std::cout << "variable : " << env_variable << " is empty!" << std::endl;
            return std::string("");
        }
        return std::string(res);
    }
};

#endif  // MICROSERVICES__CORE__SRC__PRIVATE__ENV_H
