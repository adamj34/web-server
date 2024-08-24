#include "headers/header_handler.hpp"
#include "headers/header_handler_strategy.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <memory>

HeaderHandler::HeaderHandler()
    : m_default_strategies{}
    , m_custom_strategies{} {

    // add custom strategies
    m_custom_strategies["Accept-Encoding"] = std::make_unique<AcceptEncodingHeaderHandler>();

    // add default strategies
    m_default_strategies["Date"] = std::make_unique<DateHeaderHandler>();
    m_default_strategies["X-Powered-By"] = std::make_unique<XPoweredByHeaderHandler>();
}

void HeaderHandler::handle_headers(const http::Request& request, http::Response& response) {
    for (const auto& strategy : m_default_strategies) {
        strategy.second->handle(request, response);
    }

    const auto& headers = request.get_headers();
    for (const auto& header : headers) {
        if (m_custom_strategies.contains(header.first)) {
            m_custom_strategies[header.first]->handle(request, response);
        }
    }
}
