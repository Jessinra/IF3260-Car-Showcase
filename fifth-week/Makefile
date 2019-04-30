all: clean build/main run

.PHONY: run clean

LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

build/main: main.cpp lib/* modules/glad.c
	g++ main.cpp $(CPPFLAGS) $(LIBS) model/Model.cpp modules/glad.c lib/*.cpp  -o build/main

run: build/main
	build/main

clean:
	rm -rf build/*
