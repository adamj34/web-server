#include <iostream>
#include "include/http_server.hpp"
#include "include/http_request.hpp"


int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // http::Server server{"127.0.0.1", 4221, 5};
    // server.startListening();


    http::Request req("POST /ala HTTP/1.1 \r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello,\n world");
    std::cout << req.to_string();

    return 0;
}
