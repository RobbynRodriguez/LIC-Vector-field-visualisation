#version 330 core
out vec4 FragColor;
uniform sampler2D tex;
in vec2 TextCoords;

void main(){
    vec3 result = vec3(texture(tex,TextCoords));
    FragColor = vec4(result, 1.0);
}