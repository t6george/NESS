cpu_EXEC = cpu

cpubuild:
	g++ -std=c++14 -o $(cpu_EXEC) MOS6502/cpu.cpp

cpurun:
	./$(cpu_EXEC) cpu $(rom)
