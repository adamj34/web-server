#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <unordered_map>

namespace http {

struct Response {
    std::string http_version;
    std::string status;
    std::string message;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::string to_string();
};

} // namespace http

#endif // HTTP_RESPONSE_HPP
