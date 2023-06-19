#ifndef UDP_SERVER
#define UDP_SERVER

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

class UDP_Server
{
private:
    // 16 bit port number
    in_port_t port_number = 0;
    // main_socket file descriptor
    int main_socket = 0;
    /* Starts the server */
    int start();
    /* Starts the server */
    void shutdown();

public:
    /* constructor */
    UDP_Server(in_port_t port_number);
    /* destructor */
    ~UDP_Server();
    /* start listening for connections */
    void start_listening();
};

#endif