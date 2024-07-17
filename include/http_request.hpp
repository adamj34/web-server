#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <unordered_map>

namespace http {

struct Request {
    std::string raw_request;
    std::string http_version;
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    explicit Request(const std::string& raw_request);

    std::string to_string();
    void parse();
};

} // namespace http

#endif // HTTP_REQUEST_HPP
