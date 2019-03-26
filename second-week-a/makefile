.PHONY: all clean

all: main assets/dragon.mesh

clean:
	rm -f main *.o assets/*.mesh

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
LIBS = -lglfw -lm -ldl
PY3 = python3
CP = cp
MESH_COMPONENTS = ../Asset/Dragon/dragon_horn.txt ../Asset/Dragon/dragon_head.txt ../Asset/Dragon/dragon_chest.txt ../Asset/Dragon/dragon_wing.txt ../Asset/Dragon/dragon_limb.txt ../Asset/Dragon/dragon_body.txt ../Asset/Dragon/dragon_tail.txt

main: main.cpp lib/*.cpp include/glad.c
	$(CXX) $(CXXFLAGS) main.cpp lib/*.cpp include/glad.c -o main $(LIBS)

assets/dragon.mesh: ../Asset/asset_to_binary.py $(MESH_COMPONENTS)
	$(PY3) ../Asset/asset_to_binary.py $@ $(MESH_COMPONENTS)
