#include "http_server.hpp"

#include <iostream>
#include <unistd.h>
#include <format>

namespace {
const int BUFFER_SIZE = 30720;

void log(const std::string& msg) { std::cout << msg << std::endl; }

void exitWithError(const std::string& errorMessage) {
    log("ERROR: " + errorMessage);
    exit(1);
}
} // namespace

namespace http {

TcpServer::TcpServer(const std::string& ip_address, int port)
    : m_ip_address(ip_address),
      m_port(port),
      m_socket(),
      m_client_socket(),
      m_incomingMessage(),
      m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress)),
      m_serverMessage(buildResponse())
    {
        // set up the server's socket address structure
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        // memset(m_socketAddress.sin_zero, '\0', sizeof(m_socketAddress.sin_zero));
        std::fill(std::begin(m_socketAddress.sin_zero), std::end(m_socketAddress.sin_zero), '\0');

        if (startServer() != 0) {
            std::string msg = std::format("Failed to start server with PORT: {}",
                                        ntohs(m_socketAddress.sin_port));
            log(msg);
        }
    }

TcpServer::~TcpServer() { closeServer(); }

int TcpServer::startServer() {
    // create server's socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        exitWithError("Cannot create socket");
        return 1;
    }

    // bind the socket to the address and port
    if (bind(m_socket, (sockaddr*) &m_socketAddress, m_socketAddress_len) < 0) {
        exitWithError("Cannot bind socket to address");
        return 1;
    }

    return 0;
}

void TcpServer::closeServer() {
    close(m_socket);
    close(m_client_socket);
    exit(0);
}

void TcpServer::startListening() {
    // listen for incoming connections (up to 20)
    if (listen(m_socket, 20) < 0) {
        exitWithError("Socket listen failed");
    }

    std::string msg =
        std::format("\n*** Listening on ADDRESS: {} PORT: {} ***\n\n", 
                                    inet_ntoa(m_socketAddress.sin_addr), 
                                    ntohs(m_socketAddress.sin_port));
    log(msg);

    // accept incoming connections
    int bytesReceived {};
    while (true) {
        log("====== Waiting for a new connection ======\n\n\n");
        acceptConnection();

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(m_client_socket, buffer, BUFFER_SIZE);
        if (bytesReceived < 0) {
            exitWithError("Failed to read bytes from client socket connection");
        }

        std::string msg = "------ Received Request from client ------\n\n";
        log(msg);
        log(buffer);

        sendResponse();

        close(m_client_socket);
    }
}

void TcpServer::acceptConnection() {
    // create a new socket with a connection thread
    m_client_socket =
        accept(m_socket, (sockaddr*) &m_socketAddress, &m_socketAddress_len);
    if (m_client_socket < 0) {
        std::string msg = std::format("Server failed to accept incoming connection from ADDRESS: {}; PORT: {}", 
                                    inet_ntoa(m_socketAddress.sin_addr), 
                                    ntohs(m_socketAddress.sin_port));
        exitWithError(msg);
    }
}

std::string TcpServer::buildResponse() {
    std::string htmlFile =
        "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from "
        "your Server :) </p></body></html>";
    
    std::string msg = std::format("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: {}\n\n{}",
                                htmlFile.size(), 
                                htmlFile);
    return msg;
}

void TcpServer::sendResponse() {
    long bytesSent {};

    bytesSent =
        write(m_client_socket, m_serverMessage.c_str(), m_serverMessage.size());

    if (bytesSent == m_serverMessage.size()) {
        log("------ Server Response sent to client ------\n\n");
    } else {
        log("Error sending response to client");
    }
}

} // namespace http
