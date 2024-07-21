#include "http_response.hpp"
#include <string>
#include <sstream>


namespace http {

std::string Response::to_string() {
    if (http_version.empty() || status.empty() || message.empty()) {
        throw std::runtime_error("HTTP version, status, and message must be set");
    }
    std::ostringstream oss {};
    oss << http_version << " " << status << " " << message << "\r\n";
    for (const auto& [header_name, header_value] : headers) {
        oss << header_name << ": " << header_value << "\r\n";
    }
    oss << "\r\n";
    oss << body;
    
    return oss.str();
}

} // namespace http






// // Status line
// HTTP/1.1  // HTTP version
// 200       // Status code
// OK        // Optional reason phrase
// \r\n      // CRLF that marks the end of the status line

// // Headers (empty)
// \r\n      // CRLF that marks the end of the headers

// // Response body (empty)
