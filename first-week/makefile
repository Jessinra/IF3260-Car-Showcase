all: out
clean: rm -rf out *.o

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

out: 
	$(CXX) $(CXXFLAGS) main.cpp src/glad.c -o out $(LIBS)