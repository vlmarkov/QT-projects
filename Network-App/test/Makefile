all: main

debug: main.cpp
	g++ -g -Wall -o debug mian.cpp file-manager.cpp -std=c++17

main: main.cpp file-manager.o
	g++ -Wall -O2 -o main main.cpp file-manager.o -std=c++17

file-manager: file-manager.cpp file-manager.hpp
	g++ -Wall -O2 -c file-manager.cpp -std=c++17

clean:
	rm -f main *.o