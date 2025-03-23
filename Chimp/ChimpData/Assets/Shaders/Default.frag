#version 410 core

in OutputVertex {
	vec3 ViewPosition;
	vec3 Normal;
	vec2 TexCoords;
} inVert;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;

struct PointLight {
	vec3 Position;
	float Padding;
	vec3 Color;
	float Padding2;
};

struct DirectionalLight {
	vec3 Direction; // direction light is pointing
	float Padding;
	vec3 Color;
	float Padding2;
};

layout (std140) uniform SceneLighting {
	vec3 AmbientLight;
	float Padding;
	PointLight PointLights[1];
	DirectionalLight DirectionalLights[1];
	int NumPointLights;
	int NumDirectionalLights;
	vec2 Padding2;
};

vec3 FragToLight(vec3 lightPos) {
	return normalize(lightPos - inVert.ViewPosition);
}

vec3 GetDiffuse(vec3 lightDir, vec3 lightColor) {
	return max(dot(inVert.Normal, lightDir), 0.0) * lightColor;
}

vec3 CalculatePointLight(PointLight light) {
	vec3 lightDir = FragToLight(light.Position);
	vec3 diffuse = GetDiffuse(lightDir, light.Color);

	return diffuse;
}

vec3 CalculateDirectionalLight(DirectionalLight light) {
	vec3 lightDir = light.Direction; // idk why we can do this (this should be negative)? but it works like this
	vec3 diffuse = GetDiffuse(lightDir, light.Color);

	return diffuse;
}

void main()
{
    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	vec3 light = AmbientLight;

	for (int i = 0; i < NumPointLights; ++i) {
		light += CalculatePointLight(PointLights[i]);
	}

	for (int i = 0; i < NumDirectionalLights; ++i) {
		light += CalculateDirectionalLight(DirectionalLights[i]);
	}

	FragColor.rgb *= light;
}
