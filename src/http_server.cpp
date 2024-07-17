#include "http_server.hpp"
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

const int BUFFER_SIZE = 30720;

namespace http {

Server::Server(std::string_view ip_address, int port, int connection_backlog)
    : m_ip_address(ip_address),
      m_port(port),
      m_connection_backlog(connection_backlog),
      m_valid_paths({"/index.html", "/", "/echo", "/user-agent"}) {
    if (m_port <= 0 || m_port > 65535) {
        throw std::invalid_argument("Port number is out of valid range");
    }

    // Initialize server address structure
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(m_port);
    std::fill(std::begin(m_server_addr.sin_zero), std::end(m_server_addr.sin_zero), '\0');

    // Attempt to start the server
    if (startServer() != 0) {
        std::cerr << "Failed to start server\n";
        throw std::runtime_error("Failed to start server");
    }
}

std::string Server::extractURLPath(const std::vector<std::string>& parsed_request) {
    return parsed_request[1];
}

std::string Server::extractHeader(const std::string& header_name, const std::string& request) {
    std::regex regex_pattern("User-Agent: (.*)\r\n");
    std::smatch match;

    if (std::regex_search(request, match, regex_pattern) && match.size() > 1) {
        return match.str(1);
    } else {
        return "";
    }
}

int Server::startServer() {
    m_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    int reuse = 1;
    if (setsockopt(m_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
    }

    if (bind(m_server_socket_fd, (struct sockaddr *) &m_server_addr, sizeof(m_server_addr)) != 0) {
        std::cerr << "Failed to bind to PORT " << m_port << "\n";
        return 1;
    }

    return 0;
}

int Server::acceptConnection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len) {
    int client_socket_fd = accept(m_server_socket_fd, m_client_addr, m_client_addr_len);
    if (client_socket_fd < 0) {
        std::cerr << "accepting client connection failed\n";
        exit(1);
    }
    return client_socket_fd;
}

void Server::requestHandler(int client_socket_fd) {
      int bytesReceived {};
      char buffer[BUFFER_SIZE] = {0};
      bytesReceived = read(client_socket_fd, buffer, BUFFER_SIZE);
      if (bytesReceived < 0) {
        std::cerr << "Failed to read from client\n";
        exit(1);
      }
      std::cout << "Request from client: " << buffer << std::endl;
      std::vector<std::string> parsed_req = parse_request(buffer);

      std::string m_message {};
      std::string url_path = extractURLPath(parsed_req);
      std::string ok_message = "HTTP/1.1 200 OK\r\n\r\n";
      std::string ok_message_2 = "HTTP/1.1 200 OK\r\n";

      if (url_path == "/index.html") {
        m_message = ok_message;
      } else if (url_path == "/") {
        m_message = ok_message;
      } else if (url_path.starts_with("/echo")) {
        std::string suffix_path = extractSuffixPath(url_path);
        m_message = ok_message_2 + "Content-Type: text/plain\r\n" + "Content-Length: " + std::to_string(suffix_path.length()) + "\r\n\r\n" + suffix_path;
      } else if (url_path == "/user-agent") {
        std::string user_agent = extractHeader("User-Agent", buffer);
        m_message = ok_message_2 + "Content-Type: text/plain\r\n" + "Content-Length: " + std::to_string(user_agent.length()) + "\r\n\r\n" + user_agent;
      } else if (url_path.starts_with("/files")) {
        std::string suffix_path = extractSuffixPath(url_path);
        std::string file_path = "/tmp/data/codecrafters.io/http-server-tester/" + suffix_path;
        if (std::filesystem::exists(file_path)) {
          std::ifstream fin(file_path);
          if (fin.fail()) {
            std::cerr << "Failed to open file\n";
            exit(1);
          }
          std::ostringstream oss;
          oss << fin.rdbuf();
          std::string file_content = oss.str();
          std::cout << "File content: " << file_content << std::endl;
          auto file_size = std::filesystem::file_size(file_path);
          m_message = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: " + std::to_string(file_size) + "\r\n\r\n" + file_content;
        } else {
          m_message = "HTTP/1.1 404 Not Found\r\n\r\n";
        }
      } else {
        m_message = "HTTP/1.1 404 Not Found\r\n\r\n";
      }

      write(client_socket_fd, m_message.c_str(), m_message.size());

      close(client_socket_fd);
}

int Server::startListening() {
    // start listening and limit the number of connections
    if (listen(m_server_socket_fd, m_connection_backlog) != 0) {
      std::cerr << "listen failed\n";
      return 1;
    }
    std::cout << "Server listening on port " << m_port << std::endl;

    // accept incoming connections
    while (true) {
      struct sockaddr_in m_client_addr;
      int m_client_addr_len = sizeof(m_client_addr);
      int client_socket_fd = acceptConnection((struct sockaddr *) &m_client_addr, (socklen_t *) &m_client_addr_len);

      std::thread t(&Server::requestHandler, this, client_socket_fd);
      t.detach();
    }

    return 0;
}

std::string Server::extractSuffixPath(std::string_view url_path) {
    int last_slash = url_path.find_last_of("/");
    return std::string(url_path.substr(last_slash + 1));
}


std::vector<std::string> Server::parse_request(const char* buffer) {
    std::vector<std::string> m_parsed_http_req {};
    // Parse the HTTP m_request to extract the URL path
    std::string request(buffer);
    std::istringstream iss(request);
    std::string token;
    while (iss >> token) {
      m_parsed_http_req.push_back(token);
    }
    return m_parsed_http_req;
}

Server::~Server() {
    close(m_server_socket_fd);
    exit(0);
}

} // namespace http
