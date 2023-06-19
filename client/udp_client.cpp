// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

using message_size_t = uint;

#define PORT 8002
const int BUFFER_SIZE = 100;

int main()
{
	int client_fd = 0;
	struct sockaddr_in serv_addr;
	if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout<<"\n Socket creation error" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cout<<"\nInvalid address/ Address not supported" << std::endl;
		return -1;
	}

	uint8_t data[BUFFER_SIZE];

	std::string message = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.\n There are many variations of passages of Lorem Ipsum available, but the majority have suffered alteration in some form, by injected humour, or randomised words which don't look even slightly believable. If you are going to use a passage of Lorem Ipsum, you need to be sure there isn't anything embarrassing hidden in the middle of text. All the Lorem Ipsum generators on the Internet tend to repeat predefined chunks as necessary, making this the first true generator on the Internet. It uses a dictionary of over 200 Latin words, combined with a handful of model sentence structures, to generate Lorem Ipsum which looks reasonable. The generated Lorem Ipsum is therefore always free from repetition, injected humour, or non-characteristic words etc. \n There are many variations of passages of Lorem Ipsum available, but the majority have suffered alteration in some form, by injected humour, or randomised words which don't look even slightly believable. If you are going to use a passage of Lorem Ipsum, you need to be sure there isn't anything embarrassing hidden in the middle of text. All the Lorem Ipsum generators on the Internet tend to repeat predefined chunks as necessary, making this the first true generator on the Internet. It uses a dictionary of over 200 Latin words, combined with a handful of model sentence structures, to generate Lorem Ipsum which looks reasonable. The generated Lorem Ipsum is therefore always free from repetition, injected humour, or non-characteristic words etc. *** THE END ***";

	int buffer_index = 0;
	int message_len = (int)message.length();
	
	for(int index = 0; index < message_len; index++)
	{
		data[buffer_index] = message[index];
		++buffer_index;

		if(buffer_index == BUFFER_SIZE || index == message_len - 1)
		{
			std::cout<<"Sending upto: "<<index <<std::endl;
			message_size_t s = buffer_index <  BUFFER_SIZE ? buffer_index : BUFFER_SIZE;
			sendto(client_fd, &data, s, 0, (sockaddr *)&serv_addr, sizeof(serv_addr));
			buffer_index = 0;
		}
	}
	std::cout<<("Whole message is sent.\n");

	//closing the connected socket
	close(client_fd);
	return 0;
}
