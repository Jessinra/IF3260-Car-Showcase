all: 
	make clean
	make compile 
	make run

LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

compile: main.cpp modules/glad.c
	g++ main.cpp $(LIBS) model/Model.cpp modules/glad.c lib/*.cpp  -o build/main

run: build/main
	build/main

clean:
	rm -rf build/*