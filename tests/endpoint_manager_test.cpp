#include "endpoint_manager.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Endpoint manager add endpoint") {
    

    SECTION("Add endpoint with GET method") {
        endpoint_manager manager;

        std::string path = "/api/users";
        http::MethodsHelper::Method method = http::MethodsHelper::Method::GET;
        auto callback = [](const http::Request& request) {
            // Mock implementation of the callback function
            http::Response response;
            response.set_status_code("200");
            response.set_body("Hello, world!");
            return response;
        };

        manager.add_endpoint(method, path, callback);

        REQUIRE(manager.endpoint_exists(method, path));
    }

    SECTION("Add endpoint with POST method") {
        endpoint_manager manager;

        std::string path1 = "/api/users";
        http::MethodsHelper::Method method = http::MethodsHelper::Method::POST;
        auto callback1 = [](const http::Request& request) {
            // Mock implementation of the callback function
            http::Response response{};
            response.set_body("User created successfully");
            response.set_status_code("201");
            response.set_body("User created successfully");
            return response;
        };

        std::string path2 = "/api/users/1";
        auto callback2 = [](const http::Request& request) {
            // Mock implementation of the callback function
            http::Response response{};
            response.set_status_code("200");
            response.set_body("Hello, world!");
            return response;
        };

        manager.add_endpoint(method, path1, callback1);
        manager.add_endpoint(method, path2, callback2);

        REQUIRE(manager.endpoint_exists(method, path1));
        REQUIRE(manager.endpoint_exists(method, path2));
    }
}
