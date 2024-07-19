#include <iostream>
#include "include/http_server.hpp"
#include "include/http_request.hpp"

int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    http::Server server{"127.0.0.1", 4221, 5};
    server.startListening();

    return 0;
}
