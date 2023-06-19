#ifndef TCP_SERVER
#define TCP_SERVER

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

class TCP_Server
{
private:
    // 16 bit port number
    in_port_t port_number = 0;
    //main_socket file descriptor
    int main_socket = 0;

    /* Starts the server */
    int start();
    /* Starts the server */
    void shutdown();
    /* Accepts connection */
    uint16_t accept_connection(int &socket);
    /* send the reponse */
    void send_acknowledgement(int socket, uint8_t size_received);
    /* Do something with the data on stream */
    void do_work(int socket, uint16_t client_port);
    void send_welcome_note(int socket);

public:
    /* constructor */
    TCP_Server(in_port_t port_number);
    /* destructor */
    ~TCP_Server();
    /* start listening for connections */
    void start_listening();
};

#endif