
namespace http {

enum class Method {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    UNKNOWN
};


constexpr Method methodToStr(std::string_view methodStr) {
    if (methodStr == "GET") return Method::GET;
    if (methodStr == "POST") return Method::POST;
    if (methodStr == "PUT") return Method::PUT;
    if (methodStr == "PATCH") return Method::PATCH;
    if (methodStr == "DELETE") return Method::DELETE;
    return Method::UNKNOWN; 
}

constexpr bool requiresBody(Method method) {
        return method == Method::POST || method == Method::PUT || method == Method::PATCH;
    }

} // namespace http
