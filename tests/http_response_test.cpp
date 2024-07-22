#include "http_response.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Response to_string") {
    SECTION("Valid response with body") {
        http::Response response;
        response.http_version = "HTTP/1.1";
        response.status = "200";
        response.message = "OK";
        response.headers["Content-Type"] = "text/plain";
        response.body = "Hello, World!";

        std::string expected = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/plain\r\n"
                               "\r\n"
                               "Hello, World!";

        REQUIRE(response.to_string() == expected);
    }

    SECTION("Valid response without body") {
        http::Response response;
        response.http_version = "HTTP/1.1";
        response.status = "404";
        response.message = "Not Found";
        response.headers["Content-Type"] = "text/plain";

        std::string expected = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/plain\r\n"
                               "\r\n";

        REQUIRE(response.to_string() == expected);
    }

    SECTION("Missing required fields") {
        http::Response response;
        // Assuming the response object requires all three: version, status, and message
        // Here, we deliberately omit setting the message to trigger the exception
        response.http_version = "HTTP/1.1";
        response.status = "200"; 

        REQUIRE_THROWS_AS(response.to_string(), std::runtime_error);
    }
}
