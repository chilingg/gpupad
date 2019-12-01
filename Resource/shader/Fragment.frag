#version 430 core

in vec2 TexCoor;
out vec4 outColor;

uniform sampler2D bgColor;

void main(void)
{
    outColor = texture(bgColor, TexCoor) + texture(texts, TexCoor);
}
