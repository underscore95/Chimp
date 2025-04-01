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
	int numSpotlights;
	vec3 padding;
	mat4 spotlightMatrices[1];
};

out OutputVertex {
	vec3 ViewPosition;
	vec3 Normal;
	vec2 TexCoords;
	vec4 SpotlightPosition[1];
} outVert;

void main()
{
	vec4 viewPosition = (view * model * vec4(position.x, position.y, position.z, 1.0f));
	gl_Position = projection * viewPosition;

	outVert.ViewPosition = viewPosition.xyz / viewPosition.w;
	outVert.Normal = mat3(normalMatrix) * normal;
	outVert.TexCoords = texCoords;

	for (int i = 0; i < numSpotlights; ++i) {
		outVert.SpotlightPosition[i] = spotlightMatrices[i] * vec4(outVert.ViewPosition.x, outVert.ViewPosition.y, outVert.ViewPosition.z, 1.0f);
	}
}
