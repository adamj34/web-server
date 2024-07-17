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
    std::string m_message;
    std::string m_request;
    std::string m_url_path;
    std::string suffix_path;
    std::string ok_message;
    std::string ok_message_2;

public:
    Server(std::string_view ip_address, int port, int connection_backlog);
    std::string extractURLPath(const std::vector<std::string>& parsed_request);
    std::string extractHeader(const std::string& header_name, const std::string& request);
    int startServer();
    int acceptConnection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len);
    void requestHandler(int client_socket_fd);
    int startListening();
    std::string extractSuffixPath(std::string_view url_path);
    std::vector<std::string> parse_request(const char* buffer);
    ~Server();
};

} // namespace http

#endif // SERVER_HPP
