#version 430 core

layout(points) in;
layout(line_strip, max_vertices = 4) out;

in vec2 size[1];
out vec3 color;

void main(void)
{
    vec4 position = gl_in[0].gl_Position;

    gl_Position = position;
    EmitVertex();
    gl_Position = position + vec4(0.0, size[0].y, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(size[0], 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(size[0].x, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
