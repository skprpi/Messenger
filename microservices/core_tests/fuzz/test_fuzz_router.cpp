
#include <iostream>
#include <unordered_set>

#include "core/http_assert.h"
#include "core/router.h"
#include "utils.h"

// precondition: valid url
// /some/{id}/hh POST -> /some/$/hh|POST
std::string getStandartizedUrl(const std::string& url, const std::string& http_method) {
    LOGIC_ASSERT(validUrl(url), "Expected valid url");
    size_t pos = 0; // 1 position of "/"

    std::function<size_t(size_t)> getPos2 = [&url] (size_t pos) {
        size_t pos2 = url.find('/', pos + 1);
        if (pos2 == std::string::npos && pos < url.size()) {
            pos2 = url.size();
        }
        return pos2;
    };

    size_t pos2 = getPos2(pos);
    std::string result = "";

    while(pos2 != std::string::npos) {
        const int substr_length = pos2 - pos - 1;
        std::string substr = url.substr(pos + 1, substr_length);
        LOGIC_ASSERT(substr.size() > 0, "Wrong url got!");

        if (substr[0] == '{') {
            // pattern node
            result.append("/$"); // $ means pattern
        } else {
            // named node
            result.append("/" + std::move(substr));
        }

        std::swap(pos, pos2);
        pos2 = getPos2(pos);
    }
    result.append("|" + http_method);
    return result;
}


// TODO: add http_method validation
void checkHandleFunc(const std::string& endpoint, const std::string& http_method) {
    static std::unordered_set<std::string> urls;
    static TargetResolver target_resolver;
    if (!validUrl(endpoint)) {
        return;
    }
    std::string standartized_string = getStandartizedUrl(endpoint, http_method);
    auto it = urls.insert(standartized_string);
    bool inserted = it.second;

    if (inserted) {
        // new url
        std::string endpoint_copy = endpoint;
        std::string http_method_copy = http_method;
        target_resolver.addEndpoint(std::move(endpoint_copy), std::move(http_method_copy), fake_endpoint_func);
    } else {
        // url already exist
        bool cought = false;

        // check that can't insert already exist url
        try {
            std::string endpoint_copy = endpoint;
            std::string http_method_copy = http_method;
            target_resolver.addEndpoint(std::move(endpoint_copy), std::move(http_method_copy), fake_endpoint_func);
        } catch (LogicException&) {
            cought = true;
        }
        LOGIC_ASSERT(cought, "Expected exception for same url");

        // check that can find exist url
        std::string endpoint_copy = endpoint;
        std::string http_method_copy = http_method;
        target_resolver.getCallback(std::move(endpoint_copy), std::move(http_method_copy));
    }
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    size_t url_size = size % 50 + 1; // limit max url size 
    std::string url = "";
    if (data[0] % 3 == 0) {
        url = createRandomValidUrl(data, url_size);
    } else {
        url = createRandomUrl(data, url_size);
    }
    // std::cout << "random url: " << url << std::endl;
    std::string method = createRandomValidHttpMethod(data[0]);
    checkHandleFunc(url, method);
    return 0;
}
