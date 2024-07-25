#include "http_server.hpp"
#include "endpoint_manager.hpp"
#include "http_methods_helper.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "thread_pool.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <regex>
#include <sstream>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_set>
#include <vector>

constexpr int BUFFER_SIZE = 30720;

namespace http {

Server::Server(const std::string ip_address, const int port, int connection_backlog)
    : m_ip_address{ std::move(ip_address) }
    , m_port{ port }
    , m_connection_backlog{ connection_backlog } {
    if (m_port <= 0 || m_port > 65535) {
        throw std::invalid_argument("Port number is out of valid range");
    }
    if (m_connection_backlog <= 0) {
        throw std::invalid_argument("Connection backlog must be positive");
    }

    // Initialize server address structure
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(m_port);
    std::fill(std::begin(m_server_addr.sin_zero), std::end(m_server_addr.sin_zero), '\0');

    // Attempt to start the server
    try {
        start_server();
    } catch (...) {
        std::cerr << "Failed to start server\n";
        throw;
    }
}

void Server::start_server() {
    m_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        throw std::runtime_error("Failed to create server socket");
    }

    // Allow the socket to be reused immediately after the server is closed
    int reuse = 1;
    if (setsockopt(m_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
        throw std::runtime_error("setsockopt failed");
    }

    if (bind(m_server_socket_fd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr)) < 0) {
        std::cerr << "Failed to bind to PORT " << m_port << "\n";
        throw std::runtime_error("Failed to bind to port");
    }
}

int Server::accept_connection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len) const {
    int client_socket_fd = accept(m_server_socket_fd, m_client_addr, m_client_addr_len);
    if (client_socket_fd < 0) {
        std::cerr << "accepting client connection failed\n";
        throw std::runtime_error("Failed to accept client connection");
    }
    return client_socket_fd;
}

void Server::write_response(const int client_socket_fd, const std::string& response_str) const {
    ssize_t bytes_written = write(client_socket_fd, response_str.c_str(), response_str.size());
    if (bytes_written < 0) {
        std::cerr << "Failed to write to client\n";
        throw std::runtime_error("Failed to write to client");
    }
}

void Server::request_handler(const int client_socket_fd) const {
    int bytesReceived{ 0 };
    char buffer[BUFFER_SIZE]{ 0 };
    bytesReceived = read(client_socket_fd, buffer, BUFFER_SIZE);
    if (bytesReceived < 0) {
        std::cerr << "Failed to read from client\n";
        throw std::runtime_error("Failed to read from client");
    }
    std::cout << "Request from client: " << buffer << std::endl;

    // parse the request
    http::Request request{ buffer };
    http::MethodsHelper::Method method{ request.get_method() };
    auto& path{ request.get_path() };

    // Check if the endpoint exists
    if (!m_endpoint_manager.endpoint_exists(method, path)) {
        std::cerr << "Endpoint does not exist\n";
        throw std::runtime_error("Endpoint does not exist");
    }
    // Call the endpoint
    http::Response res{ m_endpoint_manager.get_endpoint_func(method, path)(request) };

    // write the response
    std::string str{ res.to_string() };
    write_response(client_socket_fd, str);

    close(client_socket_fd);
}

void Server::start_listening() {
    // start listening and limit the number of connections
    if (listen(m_server_socket_fd, m_connection_backlog) < 0) {
        std::cerr << "listen failed\n";
        throw std::runtime_error("Failed to listen on server socket");
    }
    std::cout << "Server listening on port " << m_port << std::endl;

    // accept incoming connections
    while (true) {
        struct sockaddr_in m_client_addr;
        int m_client_addr_len = sizeof(m_client_addr);
        int client_socket_fd = accept_connection((struct sockaddr*)&m_client_addr, (socklen_t*)&m_client_addr_len);

        std::function<void()> f = std::bind(&Server::request_handler, std::cref(*this), client_socket_fd);
        m_thread_pool.submit(f);
    }
}

void Server::register_endpoint(const std::string_view method, const std::string path,
                               std::function<http::Response(const http::Request&)> callback) {
    http::MethodsHelper::Method method_enum = http::MethodsHelper::str_to_method(method);
    m_endpoint_manager.add_endpoint(method_enum, path, callback);
}

Server::~Server() {
    close(m_server_socket_fd);
    std::exit(0);
}

} // namespace http
