#ifndef HEADER_HANDLER_STRATEGY_HPP
#define HEADER_HANDLER_STRATEGY_HPP

#include "http_request.hpp"
#include "http_response.hpp"
#include "utilities/compression.hpp"

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

#endif // HEADER_HANDLER_STRATEGY_HPP
