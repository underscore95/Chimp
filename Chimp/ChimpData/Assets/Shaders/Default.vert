#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout(std140) uniform Camera {
	mat4 view;
    mat4 projection;
};

layout (std140) uniform Model {
	mat4 model;
	mat4 normalMatrix;
};

out OutputVertex {
	vec3 ViewPosition;
	vec3 Normal;
	vec2 TexCoords;
} outVert;

void main()
{
	vec4 viewPosition = (view * model * vec4(position.x, position.y, position.z, 1.0f));
	gl_Position = projection * viewPosition;

	outVert.ViewPosition = viewPosition.xyz / viewPosition.w;
	outVert.Normal = mat3(normalMatrix) * normal;
	outVert.TexCoords = texCoords;
}
