// #include "headers/header_handler_strategy.hpp"
// #include "http_request.hpp"
// #include "http_response.hpp"
// #include "utilities/compression.hpp"

// class AcceptEncodingHeaderHandler : public HeaderHandlerStrategy {
//     public:
//         void handle(const http::Request& request, http::Response& response) override {
//             auto accept_encoding = request.get_headers().at("Accept-Encoding");
//             if (accept_encoding == "gzip") {
//                 auto compressed = compression::compress(response.get_body());
//                 response.set_body(compressed);
//                 response.set_header("Content-Encoding", "gzip");
//             }
//         }
// };
