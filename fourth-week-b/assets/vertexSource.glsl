#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texture;
layout(location = 2) in vec3 vertex_normal;

out vec2 tex;
out vec3 frag_pos;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    tex = vertex_texture;
    frag_pos = vec3(model * vec4(vertex_position, 1.0f));
    normal = mat3(transpose(inverse(model))) * vertex_normal;
    gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}