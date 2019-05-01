#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 particle_position;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);
    gl_Position = vec4
        (particle_position
         + cameraRight * vertex_position.x
         + cameraUp * vertex_position.y,
         1.0f);
    //gl_Position = vec4(particle_position + vertex_position, 1);
    gl_Position = projection * view  * gl_Position;
}
