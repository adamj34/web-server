#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "http_methods_helper.hpp"
#include <string>
#include <unordered_map>

namespace http {

class Request {
    private:
        std::string m_raw_request;
        std::string m_http_version;
        MethodsHelper::Method m_method;
        std::string m_method_str;
        std::string m_path;
        std::unordered_map<std::string, std::string> m_headers;
        std::string m_body;

    public:
        void parse();
        explicit Request(const std::string& raw_request);
        const http::MethodsHelper::Method get_method() const noexcept;
        const std::string& get_method_str() const noexcept;
        const std::string& get_path() const noexcept;
        const std::string& get_http_version() const noexcept;
        const std::unordered_map<std::string, std::string>& get_headers() const noexcept;
        const std::string& get_body() const noexcept;

        std::string to_string() const;
};

} // namespace http

#endif // HTTP_REQUEST_HPP
