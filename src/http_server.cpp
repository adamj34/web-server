#include "http_server.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "endpoint_manager.hpp"
#include "http_methods_helper.hpp"
#include "thread_pool.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <regex>
#include <thread>
#include <filesystem>
#include <fstream>
#include <string_view>

constexpr int BUFFER_SIZE = 30720;

namespace http {

Server::Server(std::string_view ip_address, int port, int connection_backlog)
    : m_ip_address(ip_address),
      m_port(port),
      m_connection_backlog(connection_backlog) {
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

    if (bind(m_server_socket_fd, (struct sockaddr *) &m_server_addr, sizeof(m_server_addr)) < 0) {
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

void Server::request_handler(int client_socket_fd) const {
	int bytesReceived = {0};
	char buffer[BUFFER_SIZE] = {0};
	bytesReceived = read(client_socket_fd, buffer, BUFFER_SIZE);
	if (bytesReceived < 0) {
		std::cerr << "Failed to read from client\n";
        throw std::runtime_error("Failed to read from client");
	}
	std::cout << "Request from client: " << buffer << std::endl;

    
	// http::Request request {buffer};
	// http::Response response_body {};
	// response_body.http_version = "HTTP/1.1";
	// response_body.status = "200";
	// response_body.message = "OK";
	// response_body.headers["Server"] = "SimpleHTTPServer";
	// response_body.body = "Hello, world from the server!";

    // std::string response_body_str {response_body.to_string()};
	// ssize_t bytes_written = write(client_socket_fd, response_body_str.c_str(), response_body_str.size());
    // if (bytes_written < 0) {
    //     std::cerr << "Failed to write to client\n";
    //     throw std::runtime_error("Failed to write to client");
    // }

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
      int client_socket_fd = accept_connection((struct sockaddr *) &m_client_addr, (socklen_t *) &m_client_addr_len);

    std::function<void()> f = std::bind(&Server::request_handler, std::cref(*this), client_socket_fd);
        m_thread_pool.submit(f);
    }

}

void Server::register_endpoint(
    const std::string_view path, const std::string_view method, std::function<void()> callback
) {
    http::MethodsHelper::Method method_enum = http::MethodsHelper::str_to_method(method);
    m_endpoint_manager.add_endpoint(path, method_enum, callback);
}

Server::~Server() {
    close(m_server_socket_fd);
    std::exit(0);
}

} // namespace http
