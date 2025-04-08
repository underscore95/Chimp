#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out; // 3 * NUM_FACES

out vec4 FragPos;

layout(std140) uniform PointLightMatrices {
	mat4 proj;
	mat4 views[6];
};

// Basically making a new vertex for each of the "cameras"
void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face; 
        for (int i = 0; i < 3; ++i) 
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = (proj * views[face]) * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}