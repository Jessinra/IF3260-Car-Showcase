#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texture;

out vec2 tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    tex = vertex_texture;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}