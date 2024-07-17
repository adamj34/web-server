
namespace http {

enum class Method {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE
};


const char* methodToStr(Method method) {
    using enum Method;
    switch (method) {
        case GET:    return "GET";
        case POST:   return "POST";
        case PUT:    return "PUT";
        case PATCH:  return "PATCH";
        case DELETE: return "DELETE";
        default:     return "UNKNOWN";
    }
}

} // namespace http
