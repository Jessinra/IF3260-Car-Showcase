.PHONY: all clean

all: main assets/jeep.mesh

clean:
	rm -f main *.o assets/*.mesh

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp
PY3 = python3
CP = cp
MESH_COMPILER = ../Asset/asset_to_binary.py
JEEP_SRC = ../Asset/Jeep/jeep_body.txt

main: main.cpp lib/*.cpp include/glad.c
	$(CXX) $(CXXFLAGS) main.cpp lib/*.cpp include/glad.c -o main $(LIBS)

assets/jeep.mesh: $(MESH_COMPILER) $(JEEP_SRC)
	$(MESH_COMPILER) $@ $(JEEP_SRC)
