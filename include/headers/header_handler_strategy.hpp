#ifndef HEADER_HANDLER_STRATEGY_HPP
#define HEADER_HANDLER_STRATEGY_HPP

#include "http_request.hpp"
#include "http_response.hpp"
#include "utilities/compression.hpp"
#include <chrono>

class HeaderHandlerStrategy {
    public:
        virtual void handle(const http::Request& request, http::Response& response) = 0;
        virtual ~HeaderHandlerStrategy() = default;
};

class AcceptEncodingHeaderHandler : public HeaderHandlerStrategy {
    public:
        void handle(const http::Request& request, http::Response& response) override {
            auto accept_encoding = request.get_headers().at("Accept-Encoding");
            if (accept_encoding == "gzip") {
                auto compressed = compression::compress(response.get_body());
                response.set_body(compressed);
                response.set_header("Content-Encoding", "gzip");
            }
        }
};

class DateHeaderHandler : public HeaderHandlerStrategy {
    public:
        void handle(const http::Request& request, http::Response& response) override {
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);

            response.set_header("Date", std::ctime(&now_time));
        }
};

class XPoweredByHeaderHandler : public HeaderHandlerStrategy {
    public:
        void handle(const http::Request& request, http::Response& response) override {
            response.set_header("X-Powered-By", "C++");
        }
};

#endif // HEADER_HANDLER_STRATEGY_HPP
