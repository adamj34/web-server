#ifndef SERVER_HPP
#define SERVER_HPP

#include "concurrency_model/thread_pool.hpp"
#include "endpoint_manager.hpp"
#include <netinet/in.h>
#include <string>
#include <unordered_set>
#include <vector>

namespace http {

class Server {
    private:
        thread_pool m_thread_pool{};
        const std::string m_ip_address{};
        int m_server_socket_fd{};
        const int m_port{};
        struct sockaddr_in m_server_addr {};
        int m_connection_backlog{};
        endpoint_manager m_endpoint_manager{};

        void start_server();
        int accept_connection(struct sockaddr* m_client_addr, socklen_t* m_client_addr_len) const;
        void request_handler(const int client_socket_fd) const;
        void write_response(const int client_socket_fd, const std::string& response_str) const;

    public:
        Server(const std::string ip_address, const int port, int connection_backlog);
        ~Server();
        void start_listening();
        void register_endpoint(const std::string_view method, const std::string path,
                               std::function<http::Response(const http::Request&)> callback);
};

} // namespace http

#endif // SERVER_HPP
