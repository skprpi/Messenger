#ifndef MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ALIAS_H
#define MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ALIAS_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

using Response = boost::beast::http::response<boost::beast::http::string_body>;
using Request = boost::beast::http::request<boost::beast::http::dynamic_body>;

#endif  // MICROSERVICES__CORE__SRC__INCLUDE__CORELIB__ALIAS_H
