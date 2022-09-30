#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>


using Response = boost::beast::http::response<boost::beast::http::dynamic_body>;
using Request = boost::beast::http::request<boost::beast::http::dynamic_body>;
