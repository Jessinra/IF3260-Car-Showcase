.PHONY: all clean

all: main assets/dragon.mesh assets/polygon.mesh

clean:
	rm -f main *.o assets/*.mesh

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lglfw -lm -ldl
PY3 = python3
CP = cp
MESH_COMPILER = ../Asset/asset_to_binary.py
DRAGON_SRC = ../Asset/Dragon/dragon_horn.txt ../Asset/Dragon/dragon_head.txt ../Asset/Dragon/dragon_chest.txt ../Asset/Dragon/dragon_wing.txt ../Asset/Dragon/dragon_limb.txt ../Asset/Dragon/dragon_body.txt ../Asset/Dragon/dragon_tail.txt
POLYGON_SRC = ../Asset/Polygon/polygon.txt

main: main.cpp lib/*.cpp include/glad.c
	$(CXX) $(CXXFLAGS) main.cpp lib/*.cpp include/glad.c -o main $(LIBS)

assets/dragon.mesh: $(MESH_COMPILER) $(DRAGON_SRC)
	$(MESH_COMPILER) $@ $(DRAGON_SRC)

assets/polygon.mesh: $(MESH_COMPILER) $(POLYGON_SRC)
	$(MESH_COMPILER) $@ $(POLYGON_SRC)
