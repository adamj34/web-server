#include "http_request.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Request parsing") {
    SECTION("Valid request") {
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

    SECTION("Request with body") {
        std::string raw_request = "POST /submit HTTP/1.1\r\n"
                                  "Content-Length: 10\r\n"
                                  "\r\n"
                                  "Hello World";
        http::Request request(raw_request);

        REQUIRE(request.method == "POST");
        REQUIRE(request.path == "/submit");
        REQUIRE(request.http_version == "HTTP/1.1");
        REQUIRE(request.headers.size() == 1);
        REQUIRE(request.headers["Content-Length"] == "10");
        REQUIRE(request.body == "Hello World");
    }

    // SECTION("Invalid request") {
    //     std::string raw_request = "INVALID REQUEST";
    //     http::Request request(raw_request);

    //     // Check that the request is not parsed successfully
    //     REQUIRE(request.method.empty());
    //     REQUIRE(request.path.empty());
    //     REQUIRE(request.http_version.empty());
    //     REQUIRE(request.headers.empty());
    //     REQUIRE(request.body.empty());
    // }
}
