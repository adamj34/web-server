#ifndef HTTP_METHODS_HELPER_HPP
#define HTTP_METHODS_HELPER_HPP

#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>

namespace http {

class MethodsHelper {
public:
    enum class Method {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
    };

    static constexpr std::string_view method_to_str(Method method) {
        switch (method) {
            case Method::GET: return "GET";
            case Method::POST: return "POST";
            case Method::PUT: return "PUT";
            case Method::PATCH: return "PATCH";
            case Method::DELETE: return "DELETE";
            default: throw std::invalid_argument("Unknown method");
        }
    }

    static constexpr Method str_to_method(std::string_view method) {
        if (method == "GET") return Method::GET;
        if (method == "POST") return Method::POST;
        if (method == "PUT") return Method::PUT;
        if (method == "PATCH") return Method::PATCH;
        if (method == "DELETE") return Method::DELETE;
        throw std::invalid_argument("Unknown method");
    }

    static constexpr bool requiresBody(Method method) {
        return method == Method::POST || method == Method::PUT || method == Method::PATCH;
    }
};

} // namespace http

#endif // HTTP_METHODS_HELPER_HPP
