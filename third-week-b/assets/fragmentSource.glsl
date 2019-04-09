#version 400

in vec2 tex;
out vec4 frag_colour;

uniform sampler2D tex_sampler;

void main() {
    frag_colour = vec4(texture(tex_sampler, tex).rgb, 1.0f);
}