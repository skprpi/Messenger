#include <gtest/gtest.h>

#include "corelib/http_assert.h"
#include "corelib/router.h"

TEST(test_router, named_node_dublication) {
    Router router;
    EndpointFunction fake_function = [](CallbackInfo &&) { return std::shared_ptr<Response>(); };
    router.handleFunc("/123/path", "GET", fake_function);
    try {
        router.handleFunc("/123/path", "GET", fake_function);
        ASSERT_TRUE(false);
    } catch (LogicException &) {
        ASSERT_TRUE(true);
    }
}

TEST(test_router, different_method_named_node) {
    Router router;
    EndpointFunction fake_function = [](CallbackInfo &&) { return std::shared_ptr<Response>(); };
    router.handleFunc("/123/path", "GET", fake_function);
    router.handleFunc("/123/path", "POST", fake_function);
}

TEST(test_router, different_pattern_and_named_node) {
    Router router;
    EndpointFunction fake_function = [](CallbackInfo &&) { return std::shared_ptr<Response>(); };
    router.handleFunc("/123/path", "GET", fake_function);
    router.handleFunc("/123/{path}", "GET", fake_function);
}

TEST(test_router, different_pattern_and_named_node_reversed) {
    Router router;
    EndpointFunction fake_function = [](CallbackInfo &&) { return std::shared_ptr<Response>(); };
    router.handleFunc("/{path}", "GET", fake_function);
    router.handleFunc("/path", "GET", fake_function);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
