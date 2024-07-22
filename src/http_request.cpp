#include "http_request.hpp"
#include "http_methods.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace http {

Request::Request(const std::string& raw_request) 
    : raw_request(raw_request) 
    {
        parse();
    };

void Request::parse() {
    std::istringstream iss{raw_request};
    if (!iss) {
        std::cerr << "Failed to create string stream from the raw request" << std::endl;
        throw std::runtime_error("Failed to create string stream from the raw request");
    }
    std::string line;

    // Parse the first line of the request based on the HTTP standard
    // eg. "HTTP/1.1 GET /index.html"
    if (std::getline(iss, line)) {
        std::istringstream first_line_iss {line};
        first_line_iss >> method >> path >> http_version;
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
        headers[header_name] = header_value;
    }

    // Parse the body
    const http::Method method_enum = http::methodToStr(method);
    if (method_enum == http::Method::UNKNOWN) {
        std::cerr << "Unknown method: " << method << std::endl;
        throw std::runtime_error("Unknown method used in the request");
    }

    if (http::requiresBody(method_enum)) {
        // Check if there is a body
        if (iss.peek() != EOF) {
            // read the rest of the request as the body
            if (!std::getline(iss, body, '\0')) {
                std::cerr << "Error parsing the body of the request" << std::endl;
                throw std::runtime_error("Error parsing the body of the request");
            } 
        }
    }
}

std::string Request::to_string() {
    if (method.empty() || path.empty() || http_version.empty()) {
        std::cerr << "Request is missing required fields" << std::endl;
        throw std::invalid_argument("Request is missing required fields");
    }

    std::ostringstream oss {};
    oss << method << " " << path << " " << http_version << "\r\n";
    for (const auto& [header_name, header_value] : headers) {
        oss << header_name << ": " << header_value << "\r\n";
    }
    oss << "\r\n";
    oss << body;

    return oss.str();
}

} // namespace http






