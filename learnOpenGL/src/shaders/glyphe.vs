#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aVector;

out VS_OUT {
    vec3 vector;
} vs_out;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.vector = normalize(vec3(vec4(normalMatrix * aVector, 0.0)));
    gl_Position = view * model * vec4(aPos, 1.0);
}