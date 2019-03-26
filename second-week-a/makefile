.PHONY: all clean

all: clean main

clean:
	rm -rf main *.o

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lglfw -lm -ldl

main: main.cpp lib/*.cpp include/glad.c
	$(CXX) $(CXXFLAGS) main.cpp lib/*.cpp include/glad.c -o main $(LIBS)
