#version 430 core

uniform vec3 aPos;
uniform vec2 aSize[3];

out vec2 size;
out vec3 color;

void main(void)
{
	switch(gl_InstanceID)
	{
		case 0: color.r = 1; break;
		case 1: color.g = 1; break;
		case 2: color.b = 1; break;
	}
	size = aSize[gl_InstanceID];
    gl_Position = vec4(aPos, 1.0);
};
