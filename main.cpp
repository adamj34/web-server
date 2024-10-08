#include "include/http_request.hpp"
#include "include/http_response.hpp"
#include "include/http_server.hpp"
#include "include/utilities/file_handler.hpp"
#include <iostream>

#include "include/utilities/compression.hpp"
#include "spdlog/spdlog.h"
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {

    // configure server's address and port
    http::Server server{ "127.0.0.1", 4221, 15 };

    // register endpoints
    server.register_endpoint("GET", "/", [](const http::Request& request) {
        http::Response response{ "200", "OK" };
        response.set_header("Content-Type", "text/html").set_header("Connection", "close");

        return response;
    });

    server.register_endpoint("GET", "/file", [](const http::Request& request) {
        // send a file to the client
        std::string s{ "test_file.txt" };
        auto resource = fileHandler::send_file(s, "./");

        http::Response response{ "HTTP/1.1", "200", "OK" };
        response.set_body(resource);
        response.set_header("Content-Type", "text/plain");

        return response;
    });

    server.register_endpoint("POST", "/write_file", [](const http::Request& request) {
        // read the body of the request and write it to a file
        fileHandler::write_file("test_write.txt", request.get_body());

        http::Response response{};
        response.set_status_code("201");
        response.set_message("OK");
        response.set_header("Content-Type", "text/plain");

        return response;
    });

    try {
        server.start_listening();
    } catch (const std::exception& e) {
        spdlog::error("Exception: {}", e.what());
    }

    return 0;
}
