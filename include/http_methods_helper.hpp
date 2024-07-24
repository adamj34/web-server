#ifndef HTTP_METHODS_HELPER_HPP
#define HTTP_METHODS_HELPER_HPP

#include <string>
#include <string_view>
#include <unordered_map>
#include <array>
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
    using enum Method;
    static constexpr std::array<Method, 5> methods = {GET, POST, PUT, PATCH, DELETE};


    static constexpr std::string_view method_to_str(Method method) {
        switch (method) {
            case GET: return "GET";
            case POST: return "POST";
            case PUT: return "PUT";
            case PATCH: return "PATCH";
            case DELETE: return "DELETE";
            default: throw std::invalid_argument("Unknown method");
        }
    }

    static constexpr Method str_to_method(std::string_view method) {
        if (method == "GET") return GET;
        if (method == "POST") return POST;
        if (method == "PUT") return PUT;
        if (method == "PATCH") return PATCH;
        if (method == "DELETE") return DELETE;
        throw std::invalid_argument("Unknown method");
    }

    static constexpr bool requires_body(Method method) {
        return method == POST || method == PUT || method == PATCH;
    }
};

} // namespace http

#endif // HTTP_METHODS_HELPER_HPP
