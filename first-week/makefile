all: out
clean: rm -rf out *.o

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

out: main.o
		$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)