#version 410 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 1) in vec2 texCoords;

layout(std140) uniform Camera {
    mat4 projectionView;
};

layout (std140) uniform Model {
	mat4 model;
};

out OutputVertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} outVert;

void main()
{
	gl_Position = projectionView * model * vec4(position.x, position.y, position.z, 1.0f);

	outVert.Position = position;
	outVert.Normal = normal;
	outVert.TexCoords = texCoords;
}
