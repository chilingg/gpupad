#version 430 core

in vec2 TexCoor;
out vec4 outColor;

uniform sampler2D tex;

void main(void)
{
    outColor = texture(tex, TexCoor);
}
