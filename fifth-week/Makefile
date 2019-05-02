run: build/main
	./build/main

.PHONY: run clean

LIBS = -lGL -lGLU -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

CFLAGS = -MT $@ -MMD -MP -MF $(patsubst %.o,%.d,$@)

build/main.o: main.cpp
	g++ $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/glad.o: modules/glad.c
	gcc $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/Model.o: model/Model.cpp
	g++ $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/%.o: objects/%.cpp
	g++ $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/%.o: lib/%.cpp
	g++ $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/main: build/main.o build/glad.o build/stbi.o build/Camera.o build/Shader.o \
	build/Model.o build/Object.o build/Car.o build/Lamp.o \
	build/Particles.o build/Rain.o build/Smoke.o build/SmokeGenerator.o
	g++ -g -o $@ $^ $(LIBS)

clean:
	rm -rf build/*

build/%.d: ;

include $(wildcard build/*.d)
