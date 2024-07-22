#include "http_request.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Request parsing") {
    SECTION("Request without body - GET") {
        std::string raw_request = "GET /index.html HTTP/1.1\r\n"
                                  "Host: example.com\r\n"
                                  "User-Agent: Mozilla/5.0\r\n"
                                  "\r\n";
        http::Request request{raw_request};

        REQUIRE(request.method == "GET");
        REQUIRE(request.path == "/index.html");
        REQUIRE(request.http_version == "HTTP/1.1");
        REQUIRE(request.headers.size() == 2);
        REQUIRE(request.headers["Host"] == "example.com");
        REQUIRE(request.headers["User-Agent"] == "Mozilla/5.0");
        REQUIRE(request.body.empty());
    }

    SECTION("Request with body - POST") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                  "Content-Length: 10\r\n"
                                  "\r\n"
                                  "Hello World";
        http::Request request{raw_request};

        REQUIRE(request.method == "POST");
        REQUIRE(request.path == "/submit");
        REQUIRE(request.http_version == "HTTP/1.1");
        REQUIRE(request.headers.size() == 1);
        REQUIRE(request.headers["Content-Length"] == "10");
        REQUIRE(request.body == "Hello World");
    }

    SECTION("Request with body - PUT") {
        std::string raw_request = "PUT /update HTTP/1.1\r\n"
                                  "Content-Length: 10\r\n"
                                  "Accept: application/json\r\n"
                                  "Origin: http://example.com\r\n"
                                  "\r\n"
                                  "Hello World";
        http::Request request{raw_request};

        REQUIRE(request.method == "PUT");
        REQUIRE(request.path == "/update");
        REQUIRE(request.http_version == "HTTP/1.1");
        REQUIRE(request.headers.size() == 3);
        REQUIRE(request.headers["Content-Length"] == "10");
        REQUIRE(request.headers["Accept"] == "application/json");
        REQUIRE(request.headers["Origin"] == "http://example.com");
        REQUIRE(request.body == "Hello World");
    }

    SECTION("Request without body - POST") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                  "Content-Length: 0\r\n"
                                  "\r\n";
        http::Request request{raw_request};

        REQUIRE(request.method == "POST");
        REQUIRE(request.path == "/submit");
        REQUIRE(request.http_version == "HTTP/1.1");
        REQUIRE(request.headers.size() == 1);
        REQUIRE(request.headers["Content-Length"] == "0");
        REQUIRE(request.body.empty());
    }

    SECTION("Requst with missing method") {
        std::string raw_request = "/submit HTTP/1.1\r\n"  // missing method
                                    "Content-Length: 11\r\n"
                                    "\r\n"
                                    "Hello World";

        REQUIRE_THROWS_AS(http::Request{raw_request}, std::runtime_error);
    }

    SECTION("Invalid request") {
        std::string raw_request = "INVALID REQUEST";

        REQUIRE_THROWS_AS(http::Request{raw_request}, std::runtime_error);
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




