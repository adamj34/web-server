#ifndef ENDPOINT_MANAGER_HPP
#define ENDPOINT_MANAGER_HPP

#include "http_methods_helper.hpp"
#include <string>
#include <unordered_map>
#include <functional>


using EndpointContainer = 
    std::unordered_map<http::MethodsHelper::Method, std::unordered_map<std::string_view, std::function<void()>>>;

class endpoint_manager {
    private:
    EndpointContainer m_endpoints;

    public:
    endpoint_manager();
    void add_endpoint( std::string_view path, http::MethodsHelper::Method method, std::function<void()> callback);
};


#endif // ENDPOINT_MANAGER_HPP
