#include "endpoint_manager.hpp"


endpoint_manager::endpoint_manager() {
    for (const auto method : http::MethodsHelper::methods) {
        m_endpoints[method] = {};
    }
}

void endpoint_manager::add_endpoint(std::string_view path, http::MethodsHelper::Method method, std::function<void ()> callback) {
    m_endpoints[method][path] = callback;
}
