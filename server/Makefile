CXXFLAGS=-std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant -O3
#CXXFLAGS=-ggdb -std=c++11 -Wpedantic -Wall -Wextra -Werror -Wzero-as-null-pointer-constant

INC = -I include
SRCS = $(wildcard src/*.cpp)

all: $(SRCS)
	@echo ----------------compiling executable program server--------------------
	g++ $(CXXFLAGS) $(SRCS) $(INC) -o server 

clean:
	@echo ----------------removing executable program server--------------------
	rm server