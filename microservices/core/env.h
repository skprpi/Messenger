#ifndef MICROSERVICES_CORE_ENV
#define MICROSERVICES_CORE_ENV

#include <iostream>
#include <string>

class Environment {
public:
    static std::string getEnv(const std::string& env_variable) {
        char* res = std::getenv(env_variable.c_str());
        if (res == nullptr) {
            std::cout << "variable : " << env_variable << " is empty!" << std::endl;
            return std::string("");
        }
        return std::string(res);
    }
};

#endif  // MICROSERVICES_CORE_ENV
