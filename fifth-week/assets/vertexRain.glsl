#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 particle_position;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = vec4(particle_position + vertex_position, 1);
    gl_Position = projection * view  * gl_Position;
}
