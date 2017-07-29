all: logger test

test: logger
	g++ -g -Wall -O2 -o test test.cpp ConsoleLogger.o -std=c++17


logger: AbstractLogger.hpp ConsoleLogger.cpp ConsoleLogger.hpp
	g++ -g -Wall -O2 -c ConsoleLogger.cpp -std=c++17

clean:
	rm -f *.o test
