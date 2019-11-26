#version 430 core

uniform vec4 aPos[3];
uniform vec2 aSize[3];

out vec2 size;
out vec3 color;

void main(void)
{
    color = vec3(0, 0, 0);
    switch(gl_InstanceID)
    {
        case 0: color.r = 1; break;
        case 1: color.g = 1; break;
        case 2: color.b = 1; break;
    }
    size = aSize[gl_InstanceID];
    gl_Position = aPos[gl_InstanceID];
};
