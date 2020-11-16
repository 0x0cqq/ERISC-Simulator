main: ./src/main.cpp ./src/simulator.cpp ./src/status.cpp
	g++ -o main ./src/simulator.cpp ./src/status.cpp ./src/main.cpp 