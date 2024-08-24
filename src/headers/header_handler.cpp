#include "headers/header_handler.hpp"
#include "headers/header_handler_strategy.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <memory>

HeaderHandler::HeaderHandler()
    : m_strategies{} {
    m_strategies["Accept-Encoding"] = std::make_unique<AcceptEncodingHeaderHandler>();
}

void HeaderHandler::handle_headers(const http::Request& request, http::Response& response) {
    const auto& headers = request.get_headers();
    for (const auto& header : headers) {
        if (m_strategies.contains(header.first)) {
            m_strategies[header.first]->handle(request, response);
        }
    }
}
