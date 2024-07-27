#include "http_response.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>


TEST_CASE("Response set_header") {

    SECTION("Add new header") {
        http::Response response("HTTP/1.1", "200", "OK");
        response.set_header("Server", "SimpleHTTPServer");

        auto expected = response.get_headers();
        REQUIRE(expected.size() == 1);
        REQUIRE(expected["Server"] == "SimpleHTTPServer");
    }

    SECTION("Add multiple headers") {
        http::Response response("HTTP/1.1", "200", "OK");
        response
            .set_header("Server", "SimpleHTTPServer")
            .set_header("Connection", "close");

        auto expected = response.get_headers();
        REQUIRE(expected.size() == 2);
        REQUIRE(expected["Server"] == "SimpleHTTPServer");
        REQUIRE(expected["Connection"] == "close");
    }

    SECTION("Update existing header") {
        http::Response response("HTTP/1.1", "200", "OK", {{"Server", "SimpleHTTPServer"}}, "Body");
        response.set_header("Server", "NewServer");

        auto expected = response.get_headers();
        REQUIRE(expected.size() == 2);
        REQUIRE(expected["Server"] == "NewServer");
        REQUIRE(expected["Content-Length"] == "4");
    }
}

TEST_CASE("Content-Length header is automatically set") {
    SECTION("Body is set via setter") {
        http::Response response("HTTP/1.1", "200", "OK");
        response.set_body("Hello, World!");

        auto expected = response.get_headers();
        REQUIRE(expected.size() == 1);
        REQUIRE(expected["Content-Length"] == "13");
    }

    SECTION("Body is set via constructor") {
        http::Response response("HTTP/1.1", "200", "OK", {}, "Hello, World!");

        auto expected = response.get_headers();
        REQUIRE(expected.size() == 1);
        REQUIRE(expected["Content-Length"] == "13");
    }
}

TEST_CASE("Response to_string") {
    SECTION("Valid response with body") {
        http::Response response("HTTP/1.1", "200", "OK", {{"Content-Type", "text/plain"}}, "Hello, World!");

        std::string expected = "HTTP/1.1 200 OK\r\n"
                               "Content-Length: 13\r\n"
                               "Content-Type: text/plain\r\n"
                               "\r\n"
                               "Hello, World!";

        REQUIRE(response.to_string() == expected);
    }

    SECTION("Valid response without body") {
        http::Response response("HTTP/1.1", "404", "Not Found");
        response.set_header("Content-Type", "text/plain");

        std::string expected = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/plain\r\n"
                               "\r\n";

        REQUIRE(response.to_string() == expected);
    }

    SECTION("Missing required fields") {
        http::Response response("HTTP/1.1", "200", ""); // Empty message to simulate missing required fields

        REQUIRE_THROWS_AS(response.to_string(), std::runtime_error);
    }
}


