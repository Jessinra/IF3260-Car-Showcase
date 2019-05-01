#version 330

out vec4 frag_colour;

in vec2 tex;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D tex_sampler;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;

void main() {
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * light_color;
    // Diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light_color;  
    // Result 
    vec3 result = (ambient + diffuse + specular) * texture(tex_sampler, tex).rgb;
    frag_colour = vec4(result, 1.0f);
}