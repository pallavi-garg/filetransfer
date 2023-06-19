#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <tcp_server.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <algorithm>

#define MAXLINE 2000
#define MAX_CLIENT_CONNECTIONS 8
const int BUFFER_SIZE = 100;

using message_size_t = uint;

TCP_Server::TCP_Server(in_port_t port_number)
{
    this->port_number = port_number;

    if (this->start() != 0)
        std::cout << "Failed to start server with port: " << port_number << std::endl;
}

TCP_Server::~TCP_Server()
{
    this->shutdown();
}

int TCP_Server::start()
{
    int optval = 1;

    // hints are filters for the getaddrinfo to get matching address infos
    struct addrinfo hints = addrinfo(), *list_all_address_infos, *candidate_address;
    hints.ai_family = AF_INET;       // IPv4 Internet protocols
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_protocol = 0;
    hints.ai_flags |= AI_PASSIVE | AI_ADDRCONFIG; // On any IP address

    getaddrinfo(nullptr, std::to_string(port_number).c_str(), &hints, &list_all_address_infos);

    for (candidate_address = list_all_address_infos; candidate_address; candidate_address = candidate_address->ai_next)
    {
        // Create a socket descriptor
        if ((main_socket = socket(candidate_address->ai_family, candidate_address->ai_socktype, candidate_address->ai_protocol)) < 0)
            continue; // Socket failed, try the next one

        // Eliminates "Address already in use" error from bind
        setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        // Bind to the server
        if (bind(main_socket, candidate_address->ai_addr, candidate_address->ai_addrlen) == 0)
            break; // Success

        close(main_socket);
    }
    return !candidate_address ? -1 : 0;
}

uint16_t TCP_Server::accept_connection(int &socket)
{
    uint16_t client_port = 0;
    //Internet socket address of connecting peer (client of this server)
    struct sockaddr_in connecting_peer_socket_address = sockaddr_in();
    unsigned int connecting_peer_address_len = sizeof(connecting_peer_socket_address);
    socket = accept(main_socket, (sockaddr *)&connecting_peer_socket_address, &connecting_peer_address_len);
    client_port = ntohs(connecting_peer_socket_address.sin_port);

    if (socket >= 0)
        std::cout << "\n\nAccepted incoming connection from ADDRESS: " << inet_ntoa(connecting_peer_socket_address.sin_addr) << "; PORT: " << client_port << std::endl;
    else
        std::cout << "\n\nServer failed to accept incoming connection from ADDRESS: " << inet_ntoa(connecting_peer_socket_address.sin_addr) << "; PORT: " << client_port << std::endl;
    
    return client_port;
}

void TCP_Server::start_listening()
{
    // Make it a listening socket ready to accept connection requests
    if (listen(main_socket, MAX_CLIENT_CONNECTIONS) < 0)
    {
        close(main_socket);
        std::cout << "Socket listen failed" << std::endl;
        return;
    }

    std::cout << "\n*** Listening on ADDRESS: " << std::endl;

    while (true)
    {
        std::cout << ("====== Waiting for a new connection ======\n\n\n");
        int socket = -1;
        uint16_t port = accept_connection(socket);
        if (port != 0)
        {
            pid_t pid = fork();
            if (pid == -1)
            {
                std::cout << ("forking failed");
                close(socket);
            }
            // Child process;
            if (pid == 0)
            {
                do_work(socket, port);
                std::cout << "\nWork done for client: "<< port << std::endl << std::endl;
                close(socket);
                break;
            }
            else
                close(socket);
        }
    }
}

void TCP_Server::send_welcome_note(int socket)
{
    std::string msg = "Welcome to the server!";
    send(socket, &msg[0], msg.length(), 0);
}

void TCP_Server::do_work(int socket, uint16_t client_port)
{
    send_welcome_note(socket);

    uint8_t buffer[BUFFER_SIZE] = {0};
    recv(socket, &buffer, sizeof(message_size_t), 0);
    message_size_t total_size_to_read = ((message_size_t *)buffer)[0];

    std::cout << "Total bytes to receive: " << total_size_to_read << std::endl;

    while (total_size_to_read > 0)
    {
        int bytesReceived = recv(socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0)
            std::cout << "Failed to read bytes from client socket connection";

        if (bytesReceived == 0)
        {
            std::cout << "Everything done! "<< std::endl;
            break;
        }
        std::string data_read = ""; 
        for (int index = 0; index < bytesReceived; index++)
            data_read += buffer[index];
        std::cout << "\nClient: "<<  client_port << " >>> " << data_read;

        //send_acknowledgement(socket, bytesReceived);
        total_size_to_read -= bytesReceived;
    }

    std::cout << std::endl;
}

void TCP_Server::send_acknowledgement(int socket, uint8_t size_received)
{
    size_t bytesSent = write(socket, &size_received, sizeof(uint8_t));

    if (bytesSent != sizeof(uint8_t))
        std::cout << "Error sending response to client";
}

void TCP_Server::shutdown()
{
    // close and not shutdown because of https://stackoverflow.com/questions/4160347/close-vs-shutdown-socket
    close(this->main_socket);
    exit(0);
}
