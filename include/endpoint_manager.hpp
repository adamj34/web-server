#ifndef ENDPOINT_MANAGER_HPP
#define ENDPOINT_MANAGER_HPP

#include "utilities/http_methods_helper.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <functional>
#include <string>
#include <unordered_map>

using EndpointContainer =
    std::unordered_map<http::MethodsHelper::Method,
                       std::unordered_map<std::string, std::function<http::Response(const http::Request&)>>>;

class endpoint_manager {
    private:
        EndpointContainer m_endpoints;

    public:
        endpoint_manager();
        void add_endpoint(http::MethodsHelper::Method method, std::string path,
                          std::function<http::Response(const http::Request&)> callback);
        bool endpoint_exists(http::MethodsHelper::Method method, std::string path) const;
        std::function<http::Response(const http::Request&)> get_endpoint_func(http::MethodsHelper::Method method,
                                                                              std::string path) const;
};

#endif // ENDPOINT_MANAGER_HPP
