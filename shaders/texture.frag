#version 330 core

in vec2 TexCoor;

uniform vec4 color;
uniform sampler2D tex;

void main(void)
{
    gl_FragColor = texture2D(tex, TexCoor) * color;
}
