#include "headers/header_handler.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("HeaderHandler handle_headers") {
    // Create a HeaderHandler instance
    HeaderHandler headerHandler{};

    // Create a sample request and response
    http::Response response{};

    int default_headers = 2;

    SECTION("No headers other than the default") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "Host: example.com\r\n"
                                  "User-Agent: Mozilla/5.0\r\n"
                                  "\r\n";
        http::Request request{ raw_request };

        headerHandler.handle_headers(request, response);

        REQUIRE(response.get_headers().size() == 2);
    }

    SECTION("Headers with existing strategies - Accept-Encoding") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "Accept-Encoding: gzip\r\n"
                                  "\r\n";
        http::Request request{ raw_request };

        response.set_body("Test body");

        headerHandler.handle_headers(request, response);

        REQUIRE(response.get_headers().at("Content-Encoding") == "gzip");
        REQUIRE(compression::decompress(response.get_body()) == "Test body");
    }

    SECTION("Headers with existing strategies - Date") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "\r\n";
        http::Request request{ raw_request };

        headerHandler.handle_headers(request, response);

        REQUIRE(response.get_headers().at("Date") != "");
    }

    SECTION("Headers with existing strategies - X-Powered-By") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "\r\n";
        http::Request request{ raw_request };

        headerHandler.handle_headers(request, response);

        REQUIRE(response.get_headers().at("X-Powered-By") == "C++");
    }
}
