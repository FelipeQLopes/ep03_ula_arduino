.PHONY: all run

all: software.exe

software.exe: software.cpp
	$(CXX) -std=c++17 -Wall -Wextra software.cpp -o software.exe

run: software.exe
	./software.exe