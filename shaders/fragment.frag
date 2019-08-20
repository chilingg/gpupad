#version 330 core

in vec3 PColor;

uniform vec3 color;

void main(void)
{
    gl_FragColor = vec4(PColor * color, 1.0);
}
