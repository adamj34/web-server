#include "http_response.hpp"
#include <string>
#include <sstream>


namespace http {

Response::Response(std::string http_version, std::string status, std::string message, std::unordered_map<std::string, std::string> headers, std::string body)
    : m_http_version{std::move(http_version)},
        m_status{std::move(status)},
        m_message{std::move(message)},
        m_headers{std::move(headers)},
        m_body{std::move(body)} {}

const std::string& Response::get_http_version() const noexcept { return m_http_version; };
const std::string& Response::get_status() const noexcept { return m_status; };
const std::string& Response::get_message() const noexcept { return m_message; };
const std::unordered_map<std::string, std::string>& Response::get_headers() const noexcept { return m_headers; };
const std::string& Response::get_body() const noexcept { return m_body; };

Response& Response::set_header(std::string header_name, std::string header_value) {
    m_headers[header_name] = header_value;
    return *this;
}


std::string Response::to_string() {
    auto http_version { get_http_version() };
    auto status { get_status() };
    auto message { get_message() };

    if (http_version.empty() || status.empty() || message.empty()) {
        throw std::runtime_error("HTTP version, status, and message must be set");
    }
    std::ostringstream oss {};
    oss << http_version << " " << status << " " << message << "\r\n";
    for (const auto& [header_name, header_value] : get_headers()) {
        oss << header_name << ": " << header_value << "\r\n";
    }
    oss << "\r\n";
    oss << get_body();
    
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
