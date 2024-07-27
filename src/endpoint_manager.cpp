#include "endpoint_manager.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "utilities/http_methods_helper.hpp"

endpoint_manager::endpoint_manager() {
    for (const auto method : http::MethodsHelper::methods) {
        m_endpoints[method] = {};
    }
}

void endpoint_manager::add_endpoint(http::MethodsHelper::Method method, std::string path,
                                    std::function<http::Response(const http::Request&)> callback) {
    m_endpoints[method][path] = callback;
}

bool endpoint_manager::endpoint_exists(http::MethodsHelper::Method method, std::string path) const {
    return m_endpoints.contains(method) && m_endpoints.at(method).contains(path);
}

std::function<http::Response(const http::Request&)>
endpoint_manager::get_endpoint_func(http::MethodsHelper::Method method, std::string path) const {
    return m_endpoints.at(method).at(path);
}
