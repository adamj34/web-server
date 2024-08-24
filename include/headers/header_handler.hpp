#ifndef HEADER_HANDLER_HPP
#define HEADER_HANDLER_HPP

#include "headers/header_handler_strategy.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class HeaderHandler {
    private:
        std::unordered_map<std::string, std::unique_ptr<HeaderHandlerStrategy>> m_custom_strategies;
        std::unordered_map<std::string, std::unique_ptr<HeaderHandlerStrategy>> m_default_strategies;

    public:
        HeaderHandler();
        void handle_headers(const http::Request& request, http::Response& response);
};

#endif // HEADER_HANDLER_HPP
