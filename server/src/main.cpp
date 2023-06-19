#include <tcp_server.h>
#include <udp_server.h>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string protocol = argv[1];
        if ("udp" == protocol)
        {
            UDP_Server server = UDP_Server(8002);
            server.start_listening();
        }
        else if ("tcp" == protocol)
        {
            TCP_Server server = TCP_Server(8001);
            server.start_listening();
        }
        else
        {
            std::cout << "Chose either 'udp' or 'tcp'." << std::endl;
        }
    }
    else
    {
        std::cout << "Chose either 'udp' or 'tcp'." << std::endl;
    }

    return 0;
}