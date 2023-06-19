CXXFLAGS=-std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant -O3
#CXXFLAGS=-ggdb -std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant

SRCS_TCP = tcp_client.cpp
SRCS_UDP = udp_client.cpp

all: $(SRCS)
	@echo ----------------compiling executable program client--------------------
	g++ $(CXXFLAGS) $(SRCS_TCP) -o tcpclient 
	g++ $(CXXFLAGS) $(SRCS_UDP) -o udpclient 

clean:
	@echo ----------------removing executable program client--------------------
	rm tcpclient
	rm udpclient