all: Mountain.cpp
	g++ --std=c++11 -pthread -Wall -Werror -Wpedantic Mountain.cpp -o mountain