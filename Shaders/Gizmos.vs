#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 mvp;

void main(){
    TexCoords = aTexCoords;
    gl_Position = mvp * vec4(aPosition, 1.0);
}