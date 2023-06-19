#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <udp_server.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <algorithm>

#define MAXLINE 2000
#define MAX_CLIENT_CONNECTIONS 8
const int BUFFER_SIZE = 100;

using message_size_t = uint;

UDP_Server::UDP_Server(in_port_t port_number)
{
    this->port_number = port_number;

    if (this->start() != 0)
        std::cout << "Failed to start server with port: " << port_number << std::endl;
}

UDP_Server::~UDP_Server()
{
    this->shutdown();
}

int UDP_Server::start()
{
    int optval = 1;

    // hints are filters for the getaddrinfo to get matching address infos
    struct addrinfo hints = addrinfo(), *list_all_address_infos, *ptr;
    hints.ai_family = AF_INET;      // IPv4 Internet protocols
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_protocol = 0;
    hints.ai_flags |= AI_PASSIVE | AI_ADDRCONFIG; // On any IP address

    getaddrinfo(nullptr, std::to_string(port_number).c_str(), &hints, &list_all_address_infos);

    for (ptr = list_all_address_infos; ptr; ptr = ptr->ai_next)
    {
        // Create a socket descriptor
        if ((main_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) < 0)
            continue; // Socket failed, try the next one

        // Eliminates "Address already in use" error from bind
        setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        // Bind to the server
        if (bind(main_socket, ptr->ai_addr, ptr->ai_addrlen) == 0)
        {
            break; // Success
        }

        close(main_socket);
    }
    if (!ptr)
        return -1;
    else
        return 0;
}

void UDP_Server::start_listening()
{
    while (true)
    {
        std::cout << "\n====== Waiting for a new connection ======\n" << std::endl;

        uint8_t buffer[BUFFER_SIZE] = {0};
        struct sockaddr_in connecting_peer_socket_address = sockaddr_in();
        unsigned int connecting_peer_address_len = sizeof(connecting_peer_socket_address);
        int received_bytes = recvfrom(main_socket, buffer, BUFFER_SIZE, 0, (sockaddr *)&connecting_peer_socket_address, &connecting_peer_address_len);

        if (received_bytes > 0)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                std::string data_read = "";
                for (int index = 0; index < received_bytes; index++)
                    data_read += buffer[index];
                std::cout << "\nClient: " << ntohs(connecting_peer_socket_address.sin_port) << " >>> " << data_read;
                break;
            }
        }
    }
}

void UDP_Server::shutdown()
{
    // close and not shutdown because of https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket
    close(this->main_socket);
    exit(0);
}
