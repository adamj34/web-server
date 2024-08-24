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

    http::Server server{ "127.0.0.1", 4221, 15 };
    server.register_endpoint("GET", "/", [](const http::Request& request) {
        http::Response response{ "200" };
        response.set_header("Content-Type", "text/html").set_header("Connection", "close");

        // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return response;
    });

    server.register_endpoint("GET", "/file", [](const http::Request& request) {
        http::Response response{ "HTTP/1.1", "200", "OK" };
        std::string s{ "test_file.txt" };
        auto resource = fileHandler::send_file(s, "./");
        response.set_body(resource);
        response.set_header("Content-Type", "text/plain");

        return response;
    });

    server.register_endpoint("POST", "/write_file", [](const http::Request& request) {
        fileHandler::write_file("test_write.txt", request.get_body());
        http::Response response{};
        response.set_status_code("201");
        response.set_message("OK");
        response.set_header("Content-Type", "text/plain");

        return response;
    });

    server.start_listening();

    return 0;
}
