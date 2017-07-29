all: CpuMonitor testCpu

testCpu: CpuMonitor.o
	g++ -g -Wall -O2 -o testCpu testCpu.cpp CpuMonitor.o -std=c++17 -pthread


CpuMonitor: AbstractSystemMonitor.hpp CpuMonitor.cpp CpuMonitor.hpp
	g++ -Wall -O2 -c CpuMonitor.cpp -std=c++17 -pthread

clean:
	rm -f *.o *.dat *.png testCpu
