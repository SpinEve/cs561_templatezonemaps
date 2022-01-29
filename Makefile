all: main workload_generator

main: main.cpp zonemaps.cpp 
	clang++ -g -std=c++11 -o $@ $^

workload_generator: workload_generator.cpp
	clang++ -g -std=c++0x -o $@ $^

clear: 
	rm main
	rm workload_generator