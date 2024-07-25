#include "http_request.hpp"
#include "http_methods_helper.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

namespace http {

Request::Request(const std::string& raw_request)
    : m_raw_request {raw_request} {
    parse();
};

const http::MethodsHelper::Method Request::get_method() const noexcept { return m_method; };
const std::string& Request::get_method_str() const noexcept { return m_method_str; };
const std::string& Request::get_path() const noexcept { return m_path; };
const std::string& Request::get_http_version() const noexcept { return m_http_version; };
const std::unordered_map<std::string, std::string>& Request::get_headers() const noexcept { return m_headers; };
const std::string& Request::get_body() const noexcept { return m_body; };

void Request::parse() {
    std::istringstream iss {m_raw_request};
    if (!iss) {
        std::cerr << "Failed to create string stream from the raw request" << std::endl;
        throw std::runtime_error("Failed to create string stream from the raw request");
    }
    std::string line {};

    // Parse the first line of the request based on the HTTP standard
    // eg. "HTTP/1.1 GET /index.html"
    if (std::getline(iss, line)) {
        std::istringstream first_line_iss {line};
        first_line_iss >> m_method_str >> m_path >> m_http_version;
        m_method = {http::MethodsHelper::str_to_method(m_method_str)};
    } else {
        std::cerr << "Error parsing the first line of the request" << std::endl;
        throw std::runtime_error("Error parsing the first line of the request");
    }

    // Parse the headers
    while (std::getline(iss, line) && !line.empty() && line != "\r") {
        // Check if the last character is a carriage return and remove it
        if (line.back() == '\r') {
            line.pop_back();
        }

        std::string header_name {};
        std::string header_value {};
        std::size_t colon_pos = line.find(':');
        header_name = line.substr(0, colon_pos);
        header_value = line.substr(colon_pos + 2);
        m_headers[header_name] = header_value;
    }

    // Parse the body
    if (http::MethodsHelper::requires_body(m_method)) {
        // Check if there is a body
        if (iss.peek() != EOF) {
            // read the rest of the request as the body
            if (!std::getline(iss, m_body, '\0')) {
                std::cerr << "Error parsing the body of the request" << std::endl;
                throw std::runtime_error("Error parsing the body of the request");
            }
        }
    }
}

std::string Request::to_string() const {
    auto http_version {get_http_version()};
    auto method_str {get_method_str()};
    auto path {get_path()};

    if (method_str.empty() || path.empty() || http_version.empty()) {
        std::cerr << "Request is missing required fields" << std::endl;
        throw std::invalid_argument("Request is missing required fields");
    }

    std::ostringstream oss {};
    oss << method_str << " " << path << " " << http_version << "\r\n";
    for (const auto& [header_name, header_value] : get_headers()) {
        oss << header_name << ": " << header_value << "\r\n";
    }
    oss << "\r\n";
    oss << get_body();

    return oss.str();
}

} // namespace http
