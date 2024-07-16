#include <iostream>
#include "http_server.hpp"

int main() {
    http::TcpServer server { http::TcpServer("127.0.0.1", 8080) };
    server.startListening();

    return 0;
}
