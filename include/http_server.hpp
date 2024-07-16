#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>

namespace http {

class TcpServer {
    private:
    std::string m_ip_address;
    int m_port;
    int m_socket;
    int m_client_socket;
    long m_incomingMessage;
    struct sockaddr_in m_socketAddress;
    unsigned int m_socketAddress_len;
    std::string m_serverMessage;

    int startServer();
    void closeServer();
    void acceptConnection();
    std::string buildResponse();
    void sendResponse();

    public:
    TcpServer(const std::string& ip_address, int port);
    ~TcpServer();
    void startListening();

};

} // namespace http

#endif
