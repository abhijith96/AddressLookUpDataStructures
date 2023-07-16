//
// Created by Sreedevi R Warrier on 2023-07-16.
//


#include <iostream>
#include <DHCPAllocator/src/HTTPServer/HttpServer.h>


std::string HttpServer::generateResponse(const http::request<http::string_body>& request) {
    std::string response;

    if (request.method() == http::verb::get) {
        if (request.target() == "/") {
            response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html><body><h1>Welcome to the DHCP server </h1></body></html>";
        } else {
            response = "HTTP/1.1 404 Not Found\r\n"
                       "Content-Type: text/plain\r\n"
                       "\r\n"
                       "404 Not Found";
        }
    } else {
        response = "HTTP/1.1 400 Bad Request\r\n"
                   "Content-Type: text/plain\r\n"
                   "\r\n"
                   "400 Bad Request";
    }

    return response;
}

int main(){


    net::io_context ioContext;

    net::ip::tcp::acceptor acceptor(ioContext, net::ip::tcp::endpoint(net::ip::tcp::v4(), PORT));

    std::cout << "HTTP Server listening on port " << PORT << std::endl;

    while (true) {

        net::ip::tcp::socket socket(ioContext);
        acceptor.accept(socket);

        // read request
        beast::flat_buffer buffer;
        http::request<http::string_body> request;
        http::read(socket, buffer, request);

        HttpServer httpServer;

        // process and generate response
        std::string response =  httpServer.generateResponse(request);

        // sending response
        http::response<http::string_body> httpResponse{http::status::ok, request.version()};
        httpResponse.set(http::field::server, "HTTP Server mimicking DHCP server");
        httpResponse.set(http::field::content_type, "text/html");
        httpResponse.body() = response;
        httpResponse.prepare_payload();

        http::write(socket, httpResponse);

        // close socket
        beast::error_code ec;
        socket.shutdown(net::socket_base::shutdown_both, ec);
        socket.close(ec);
    }

    return 0;



}