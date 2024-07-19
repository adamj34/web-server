#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <netinet/in.h>
#include <unordered_set>

namespace http {

class Server {
private:
    std::string m_ip_address;
    int m_server_socket_fd;
    int m_port;
    struct sockaddr_in m_server_addr;
    int m_connection_backlog;
    std::unordered_set<std::string> m_valid_paths;
    int startServer();
    int acceptConnection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len);
    void requestHandler(int client_socket_fd);

public:
    Server(std::string_view ip_address, int port, int connection_backlog);
    ~Server();
    int startListening();
};

} // namespace http

#endif // SERVER_HPP
