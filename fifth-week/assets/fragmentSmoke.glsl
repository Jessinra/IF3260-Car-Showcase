#version 330

in vec2 uv;

out vec4 frag_color;

uniform sampler2D tex_sampler;

void main() {
    frag_color = texture(tex_sampler, uv);
}
