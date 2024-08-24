#include "http_server.hpp"
#include "concurrency_model/thread_pool.hpp"
#include "endpoint_manager.hpp"
#include "headers/header_handler.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "spdlog/spdlog.h"
#include "utilities/http_methods_helper.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unordered_set>
#include <vector>

namespace http {

constexpr int BUFFER_SIZE = 30720;

Server::Server(const std::string ip_address, const int port, int connection_backlog)
    : m_ip_address{ std::move(ip_address) }
    , m_port{ port }
    , m_connection_backlog{ connection_backlog } {

    // check if the port number is valid
    if (m_port <= 0 || m_port > 65535) {
        spdlog::error("Port number: {} is out of valid range", m_port);
        throw std::invalid_argument("Port number is out of valid range");
    }

    if (m_connection_backlog <= 0) {
        spdlog::error("Connection backlog must be positive");
        throw std::invalid_argument("Connection backlog must be positive");
    }

    // Initialize server address structure
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
    m_server_addr.sin_port = htons(m_port);
    std::fill(std::begin(m_server_addr.sin_zero), std::end(m_server_addr.sin_zero), '\0');

    // Attempt to start the server
    try {
        start_server();
    } catch (const std::exception& e) {
        spdlog::error("Server failed to start");
        spdlog::error(e.what());
        close(m_server_socket_fd);
        throw;
    }
}

void Server::start_server() {
    m_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket_fd < 0) {
        spdlog::error("Failed to create server socket");
        throw std::runtime_error("Failed to create server socket");
    }

    // Allow the socket to be reused immediately after the server is closed
    int reuse = 1;
    if (setsockopt(m_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        spdlog::error("setsockopt failed");
        throw std::runtime_error("setsockopt failed");
    }

    if (bind(m_server_socket_fd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr)) < 0) {
        spdlog::error("Failed to bind to port {}", m_port);
        throw std::runtime_error("Failed to bind to port");
    }
}

int Server::accept_connection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len) const {
    int client_socket_fd = accept(m_server_socket_fd, m_client_addr, m_client_addr_len);
    if (client_socket_fd < 0) {
        spdlog::error("Failed to accept client connection");
        throw std::runtime_error("Failed to accept client connection");
    }
    return client_socket_fd;
}

void Server::write_response(const int client_socket_fd, const std::string& response_str) const {
    ssize_t bytes_written = write(client_socket_fd, response_str.c_str(), response_str.size());
    if (bytes_written < 0) {
        spdlog::error("Failed to write to client");
        throw std::runtime_error("Failed to write to client");
    }
}

void Server::request_handler(const int client_socket_fd) const {
    int bytesReceived{ 0 };
    char buffer[BUFFER_SIZE]{ 0 };
    bytesReceived = read(client_socket_fd, buffer, BUFFER_SIZE);
    if (bytesReceived < 0) {
        spdlog::error("Failed to read from client");
        throw std::runtime_error("Failed to read from client");
    }
    spdlog::info("Request from client:\n {}", buffer);

    // parse the request
    http::Request request{ buffer };
    http::MethodsHelper::Method method{ request.get_method() };
    auto& path{ request.get_path() };

    // Check if the endpoint exists
    if (!m_endpoint_manager.endpoint_exists(method, path)) {
        spdlog::error("Endpoint: {} does not exist", path);
        throw std::runtime_error("Endpoint does not exist");
    }
    // Call the endpoint
    http::Response response{ m_endpoint_manager.get_endpoint_func(method, path)(request) };

    // handle request headers
    HeaderHandler header_handler{};
    header_handler.handle_headers(request, response);

    // write the response
    std::string str{ response.to_string() };
    write_response(client_socket_fd, str);

    close(client_socket_fd);
}

void Server::start_listening() {
    // start listening and limit the number of connections
    if (listen(m_server_socket_fd, m_connection_backlog) < 0) {
        spdlog::error("Failed to listen on server socket");
        throw std::runtime_error("Failed to listen on server socket");
    }
    spdlog::info("Server started listening on {}:{}", m_ip_address, m_port);

    // accept incoming connections
    while (true) {
        struct sockaddr_in m_client_addr;
        socklen_t m_client_addr_len = sizeof(m_client_addr);
        int client_socket_fd = accept_connection((struct sockaddr*)&m_client_addr, (socklen_t*)&m_client_addr_len);

        // std::function<void()> f = std::bind(&Server::request_handler, std::cref(*this), client_socket_fd);
        auto f = [this, client_socket_fd]() { request_handler(client_socket_fd); };
        m_thread_pool.submit(f);
    }
}

void Server::register_endpoint(const std::string_view method, const std::string path,
                               std::function<http::Response(const http::Request&)> callback) {
    http::MethodsHelper::Method method_enum = http::MethodsHelper::str_to_method(method);
    m_endpoint_manager.add_endpoint(method_enum, path, callback);
}

Server::~Server() { close(m_server_socket_fd); }

} // namespace http
