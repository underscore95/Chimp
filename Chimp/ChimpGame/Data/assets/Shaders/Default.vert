#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

layout(std140) uniform Camera {
    mat4 projectionView;
};

layout (std140) uniform Model {
	mat4 model;
};

out vec2 v_TexCoords;

void main()
{
	gl_Position = projectionView * model * vec4(position.x, position.y, position.z, 1.0f);

	v_TexCoords = texCoords;
}
