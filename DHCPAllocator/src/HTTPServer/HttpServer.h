//
// Created by Sreedevi R Warrier on 2023-07-16.
//

#ifndef DHCPALLOCATOR_HTTPSERVER_H
#define DHCPALLOCATOR_HTTPSERVER_H


#include <string>
#include <boost/container/flat_map.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/beast/http/message.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>

const int PORT = 8080;


class HttpServer {

public:

    std::string generateResponse(const http::request<http::string_body>& request);
};


#endif //DHCPALLOCATOR_HTTPSERVER_H
