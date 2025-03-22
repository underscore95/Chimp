#version 410 core

in OutputVertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} inVert;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

struct PointLight {
	vec3 Position;
	float ConstantAttenuation;
	vec3 Colour;
	float LinearAttenuation;
};

layout (std140) uniform SceneLighting {
	int NumPointLights;
	PointLight PointLights[1];
};

void main()
{
    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	FragColor.r = NumPointLights == 1 ? 1 : 0;
}
