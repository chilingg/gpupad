#version 330 core

in vec2 TexCoor;

uniform vec4 color;
uniform sampler2D tex;

void main(void)
{
    gl_FragColor = vec4(color.rgb, texture2D(tex, TexCoor).r);
}
