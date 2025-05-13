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

layout (std140) uniform LightMatrices {
	mat4 spotlightMatrices[4];
	mat4 directionalMatrices[4];
	int numSpotlights;
	int numDirectional;
	float padding2;
	float padding3;
};

out OutputVertex {
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoords;
	vec4 SpotlightPosition[4];
	vec4 DirectionalPosition[4];
} outVert;

void main()
{
	vec4 modelPosition = model * vec4(position.x, position.y, position.z, 1.0f);
	vec4 viewPosition = (view * modelPosition);
	gl_Position = projection * viewPosition;

	outVert.WorldPosition = modelPosition.xyz / modelPosition.w;
	outVert.Normal = mat3(normalMatrix) * normal;
	outVert.TexCoords = texCoords;

	for (int i = 0; i < numSpotlights; ++i) {
		outVert.SpotlightPosition[i] = spotlightMatrices[i] * vec4(modelPosition.x, modelPosition.y, modelPosition.z, 1.0f);
	}

	for (int i = 0; i < numDirectional; ++i) {
		outVert.DirectionalPosition[i] = directionalMatrices[i] * vec4(modelPosition.x, modelPosition.y, modelPosition.z, 1.0f);
	}
}
