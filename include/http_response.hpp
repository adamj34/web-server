#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <unordered_map>

namespace http {

class Response {
    private:
        std::string m_http_version{ "HTTP/1.1" };
        std::string m_status{};
        std::string m_message{};
        std::unordered_map<std::string, std::string> m_headers{};
        std::string m_body{};

    public:
        Response() = default;
        Response(std::string http_version, std::string status, std::string message,
                 std::unordered_map<std::string, std::string> headers, std::string body);
        Response(std::string http_version, std::string status, std::string message);
        Response(std::string status);

        const std::string& get_http_version() const noexcept;
        const std::string& get_status() const noexcept;
        const std::string& get_message() const noexcept;
        const std::unordered_map<std::string, std::string>& get_headers() const noexcept;
        const std::string& get_body() const noexcept;

        const void set_http_version(std::string http_version);
        const void set_status_code(std::string status);
        const void set_message(std::string message);
        const void set_body(std::string body);

        Response& set_header(std::string header_name, std::string header_value);
        std::string to_string() const;
};

} // namespace http

#endif // HTTP_RESPONSE_HPP
