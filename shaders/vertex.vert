#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoor;
layout(location = 2) in vec3 aColor;

out vec3 TexCoor;
out vec3 PColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, .0, 1.0);
    TexCoor = aTexCoor;
    PColor = aColor;
}
