all:
	clang++ -std=c++17 -Wall -Wextra software.cpp -o software

run:
	./software