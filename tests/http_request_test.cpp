#include "http_request.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Request parsing") {
    SECTION("Request without body - GET") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "Host: example.com\r\n"
                                  "User-Agent: Mozilla/5.0\r\n"
                                  "\r\n";
        http::Request request{raw_request};

        REQUIRE(request.get_method() == http::MethodsHelper::Method::GET);
        REQUIRE(request.get_method_str() == "GET");
        REQUIRE(request.get_path() == "/index.html");
        REQUIRE(request.get_http_version() == "HTTP/1.1");
        auto headers { request.get_headers() };
        REQUIRE(headers.size() == 2);
        REQUIRE(headers["Host"] == "example.com");
        REQUIRE(headers["User-Agent"] == "Mozilla/5.0");
        REQUIRE(request.get_body().empty());
    }

    SECTION("Request with body - POST") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                  "Content-Length: 10\r\n"
                                  "\r\n"
                                  "Hello World";
        http::Request request{raw_request};

        REQUIRE(request.get_method() == http::MethodsHelper::Method::POST);
        REQUIRE(request.get_method_str() == "POST");
        REQUIRE(request.get_path() == "/submit");
        REQUIRE(request.get_http_version() == "HTTP/1.1");
        auto headers { request.get_headers() };
        REQUIRE(headers.size() == 1);
        REQUIRE(headers["Content-Length"] == "10");
        REQUIRE(request.get_body() == "Hello World");
    }

    SECTION("Request with body - PUT") {
        std::string raw_request = "PUT /update HTTP/1.1\r\n"
                                  "Content-Length: 10\r\n"
                                  "Accept: application/json\r\n"
                                  "Origin: http://example.com\r\n"
                                  "\r\n"
                                  "Hello World";
        http::Request request{raw_request};

        REQUIRE(request.get_method() == http::MethodsHelper::Method::PUT);
        REQUIRE(request.get_method_str() == "PUT");
        REQUIRE(request.get_path() == "/update");
        REQUIRE(request.get_http_version() == "HTTP/1.1");
        auto headers { request.get_headers() };
        REQUIRE(headers.size() == 3);
        REQUIRE(headers["Content-Length"] == "10");
        REQUIRE(headers["Accept"] == "application/json");
        REQUIRE(headers["Origin"] == "http://example.com");
        REQUIRE(request.get_body() == "Hello World");
    }

    SECTION("Request without body - POST") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                  "Content-Length: 0\r\n"
                                  "\r\n";
        http::Request request{raw_request};

        REQUIRE(request.get_method() == http::MethodsHelper::Method::POST);
        REQUIRE(request.get_method_str() == "POST");
        REQUIRE(request.get_path() == "/submit");
        REQUIRE(request.get_http_version() == "HTTP/1.1");
        auto headers { request.get_headers() };
        REQUIRE(headers.size() == 1);
        REQUIRE(headers["Content-Length"] == "0");
        REQUIRE(request.get_body().empty());
    }

    SECTION("Requst with missing method") {
        std::string raw_request = "/submit HTTP/1.1\r\n"  // missing method
                                    "Content-Length: 11\r\n"
                                    "\r\n"
                                    "Hello World";

        REQUIRE_THROWS_AS(http::Request{raw_request}, std::invalid_argument);
    }

    SECTION("Invalid request") {
        std::string raw_request = "INVALID REQUEST";

        REQUIRE_THROWS_AS(http::Request{raw_request}, std::invalid_argument);
    }
}

TEST_CASE("Request to_string") {
    SECTION("Valid request with body") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                    "Content-Length: 11\r\n"
                                    "\r\n"
                                    "Hello World";
        http::Request request{raw_request};

        std::string expected = "POST /submit HTTP/1.1\r\n"
                                "Content-Length: 11\r\n"
                                "\r\n"
                                "Hello World";

        REQUIRE(request.to_string() == expected);
    }

    SECTION("Valid request without body") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                    "User-Agent: Mozilla/5.0\r\n"
                                    "\r\n";
        http::Request request{raw_request};

        std::string expected = "GET /index.html HTTP/1.1\r\n"
                                "User-Agent: Mozilla/5.0\r\n"
                                "\r\n";

        REQUIRE(request.to_string() == expected);
    }

}




