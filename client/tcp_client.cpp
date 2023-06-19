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

#define PORT 8001
const int BUFFER_SIZE = 100;

int main()
{
	int status, client_fd;
	struct sockaddr_in serv_addr;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "\n Socket creation error" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cout << "\nInvalid address/ Address not supported" << std::endl;
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0)
	{
		std::cout << "\nConnection Failed" << std::endl;
		return -1;
	}

	uint8_t data[BUFFER_SIZE];

	int data_size = recv(client_fd, &data, BUFFER_SIZE, 0);

	std::cout << "\nConnected:" << data_size << std::endl;
	for (int i = 0; i < data_size; ++i)
		std::cout << data[i];

	std::cout << std::endl;
	std::string message = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.\n There are many variations of passages of Lorem Ipsum available, but the majority have suffered alteration in some form, by injected humour, or randomised words which don't look even slightly believable. If you are going to use a passage of Lorem Ipsum, you need to be sure there isn't anything embarrassing hidden in the middle of text. All the Lorem Ipsum generators on the Internet tend to repeat predefined chunks as necessary, making this the first true generator on the Internet. It uses a dictionary of over 200 Latin words, combined with a handful of model sentence structures, to generate Lorem Ipsum which looks reasonable. The generated Lorem Ipsum is therefore always free from repetition, injected humour, or non-characteristic words etc. \n There are many variations of passages of Lorem Ipsum available, but the majority have suffered alteration in some form, by injected humour, or randomised words which don't look even slightly believable. If you are going to use a passage of Lorem Ipsum, you need to be sure there isn't anything embarrassing hidden in the middle of text. All the Lorem Ipsum generators on the Internet tend to repeat predefined chunks as necessary, making this the first true generator on the Internet. It uses a dictionary of over 200 Latin words, combined with a handful of model sentence structures, to generate Lorem Ipsum which looks reasonable. The generated Lorem Ipsum is therefore always free from repetition, injected humour, or non-characteristic words etc. *** THE END ***";

	//"1 This is a long message. 2 This is a long message. 3 This is a long message. 4 This is a long message. 5 This is a long message. 6 This is a long message. 7 This is a long message. 8 This is a long message. 9 This is a long message. 10 This is a long message. 11 This is a long message. 12 This is a long message. 13 This is a long message. 14 This is a long message. 15 This is a long message. 16 This is a long message. 17 This is a long message. 18 This is a long message. 19 This is a long message. Ending now!" ;

	message_size_t size = message.length();
	send(client_fd, &size, sizeof(message_size_t), 0);
	std::cout << "Sent total size: " << message.length() << std::endl;

	int buffer_index = 0;
	int message_len = (int)message.length();

	for (int index = 0; index < message_len; index++)
	{
		data[buffer_index] = message[index];
		++buffer_index;

		if (buffer_index == BUFFER_SIZE || index == message_len - 1)
		{
			std::cout << "Sending upto: " << index << std::endl;
			message_size_t s = buffer_index < BUFFER_SIZE ? buffer_index : BUFFER_SIZE;
			send(client_fd, &data, s, 0);
			// recv(client_fd, &received_by_server, sizeof(message_size_t), 0);
			buffer_index = 0;
		}
	}
	//uint8_t end[0];
	//send(client_fd, &end, sizeof(end), 0);
	std::cout << "Whole message sent" << std::endl;

	// closing the connected socket
	close(client_fd);
	return 0;
}
