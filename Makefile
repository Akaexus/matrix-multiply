default_target: all
.PHONY : default_target

all:
	g++ -o main main.cpp -fopenmp
