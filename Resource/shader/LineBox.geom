#version 430 core

layout(points) in;
layout(line_strip, max_vertices = 5) out;

in vec2 size[1];
in vec3 color[1];
out vec3 lineColor;

void main(void)
{
    lineColor = color[0];
    vec4 position = gl_in[0].gl_Position;

    gl_Position = position;
    EmitVertex();
    gl_Position = position + vec4(0.0, size[0].y, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(size[0], 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(size[0].x, 0.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = position;
    EmitVertex();

    EndPrimitive();
}
