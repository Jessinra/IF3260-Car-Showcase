#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 particle_position;

uniform mat4 projection;
uniform mat4 view;

out vec2 uv;

void main() {
    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
    mat4 rotation;
    rotation[0] = vec4(right, 0);
    rotation[1] = vec4(up, 0);
    rotation[2] = vec4(0); // We can ignore the look vector
    rotation[3] = vec4(particle_position, 1);
    gl_Position = rotation * vec4(vertex_position * 0.02f, 1);
    gl_Position = projection * view  * gl_Position;
    uv = vec2(vertex_position);
}
