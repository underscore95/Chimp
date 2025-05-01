#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout(std140) uniform CurrentLayer {
    int currentLayer;
    float padding1;
    float padding2;
    float padding3;
};

void main()
{
    gl_Layer = currentLayer;

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
