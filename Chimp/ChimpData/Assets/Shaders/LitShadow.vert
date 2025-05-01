#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Model {
	mat4 model;
	mat4 normalMatrix;
};

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
