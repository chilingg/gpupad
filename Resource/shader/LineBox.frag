#version 430 core

in vec3 lineColor;
out vec4 outColor;

void main(void)
{
    outColor = vec4(lineColor, 1.0);
}
