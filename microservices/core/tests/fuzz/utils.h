#ifndef MICROSERVICES_CORE_TESTS_UTILS
#define MICROSERVICES_CORE_TESTS_UTILS

#include <iostream>
#include <random>
#include <unordered_set>

#include "corelib/http_assert.h"
#include "corelib/router.h"

EndpointFunction fake_endpoint_func = [](CallbackInfo &&) { return std::shared_ptr<Response>(); };

bool validUrl(const std::string &url) {
    try {
        std::string copy_url(url);
        TargetResolver resolver;
        resolver.addUrlToTrie(std::move(copy_url), "FAKE", fake_endpoint_func);
    } catch (ParseException &) {
        return false;
    }
    return true;
}

std::string getUrlAlphabetWithoutSlash() {
    std::string alphabet = "";
    for (char ch = 'a', CH = 'A'; ch <= 'z'; ++ch, ++CH) {
        alphabet += ch;
        alphabet += CH;
    }
    for (char ch = '0'; ch <= '9'; ++ch) {
        alphabet += ch;
    }
    return alphabet;
}

std::string createRandomUrl(const uint8_t *data, size_t size) {
    static const std::string alphabet = getUrlAlphabetWithoutSlash() + "/";
    std::string url;
    for (int i = 0; i < size; ++i) {
        const uint8_t num = data[i];
        const size_t idx = num % alphabet.size();
        url += alphabet[idx];
    }
    return url;
}

// postcondition: valid url
std::string createRandomValidUrl(const uint8_t *data, size_t size) {
    if (size == 0) {
        return "";
    }
    static const std::string alphabet = getUrlAlphabetWithoutSlash();
    static std::random_device random_device;
    static std::mt19937 rnd(random_device());
    static std::uniform_int_distribution<std::mt19937::result_type> suburl_length_distribution(1, 10);
    static std::uniform_int_distribution<std::mt19937::result_type> pattetn_suburl_distribution(0, 3);

    std::string url = "";
    std::unordered_set<std::string> pattern_node_names;
    for (int i = 0; i < size;) {
        std::string suburl = "";
        const uint8_t suburl_length = suburl_length_distribution(rnd);
        for (int j = 0; j < suburl_length; ++i, ++j) {
            const uint8_t num = data[i];
            suburl += alphabet[num % alphabet.size()];
        }

        const bool pattern_suburl = pattetn_suburl_distribution(rnd) == 0;  // 1/4
        auto insert_result = pattern_node_names.emplace(suburl);
        const bool inserted = insert_result.second;
        if (pattern_suburl && inserted /* have no name dublication */) {
            suburl = "{" + suburl + "}";
        }
        url += "/" + suburl;
    }
    LOGIC_ASSERT(validUrl(url), "Expected valid url. Url generated: " + url);
    return url;
}

std::string createRandomValidHttpMethod(uint8_t random_num) {
    static const std::vector<std::string> methods = {"POST", "PUT", "GET", "DELETE"};
    const size_t idx = random_num % methods.size();
    return methods[idx];
}

#endif  // MICROSERVICES_CORE_TESTS_UTILS
