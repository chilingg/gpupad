#version 430 core

in vec2 TexCoor;
out vec4 outColor;

uniform vec4 color;
uniform sampler2D tex;

void main(void)
{
    outColor = vec4(1);
}
