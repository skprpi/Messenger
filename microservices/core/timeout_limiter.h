#ifndef MICROSERVICES_CORE_BASE_TIMEOUT_LIMITER
#define MICROSERVICES_CORE_BASE_TIMEOUT_LIMITER

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <functional>
#include <memory>
#include <type_traits>

// BaseTimeoutLimiter

class BaseTimeoutLimiter {
protected:
    using tcp = boost::asio::ip::tcp;

public:
    BaseTimeoutLimiter() = default;

    virtual void start(tcp::socket &socket) = 0;

    virtual void cancel() = 0;

    virtual ~BaseTimeoutLimiter() {}
};

// TimeoutLimiter

class TimeoutLimiter final
    : public BaseTimeoutLimiter,
      public std::enable_shared_from_this<TimeoutLimiter> {
public:
    TimeoutLimiter(tcp::socket &socket, std::chrono::seconds expiry_time)
        : socket(socket), timer(socket.get_executor(), expiry_time) {}

    void start(tcp::socket &socket) final {
        auto self = this->shared_from_this();
        timer.async_wait([self](boost::beast::error_code ec) {
            if (ec) {
                return;
            }
            self->socket.close(ec);
        });
    }

    void cancel() final { timer.cancel(); }

private:
    tcp::socket &socket;
    boost::asio::steady_timer timer;
};

// EmptyTimeoutLimiter

class EmptyTimeoutLimiter final : public BaseTimeoutLimiter {
public:
    EmptyTimeoutLimiter() = default;

    void start(tcp::socket &socket) final {
        // do nothing
    }

    void cancel() final {
        // do nothing
    }
};

// TODO: use config file instead of builder
template <typename DerivedTimeLimiterType, typename... Args>
std::reference_wrapper<BaseTimeoutLimiter> buildTimeoutLimiter(Args &&...args) {
    static_assert(
        std::is_base_of<BaseTimeoutLimiter, DerivedTimeLimiterType>::value,
        "Class parameter have"
        "to be a children of BaseTimeoutLimiter");
    return *std::make_shared<BaseTimeoutLimiter>(std::forward<Args>(args)...);
}

#endif  // MICROSERVICES_CORE_BASE_TIMEOUT_LIMITER
