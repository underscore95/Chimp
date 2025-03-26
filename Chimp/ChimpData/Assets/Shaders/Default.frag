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

	vec3 Attenuation;
	float Padding3;
};

struct DirectionalLight {
	vec3 Direction; // direction light is pointing
	float Padding;

	vec3 Color;
	float Padding2;
};

struct Spotlight {
		vec3 Direction;
		float Padding;

		vec3 Position;
		float Padding2;

		vec3 Color;
		float Padding4;

		vec3 Attenuation;
		
		float CutoffAngle; // Is Cos(Angle)
	};

layout (std140) uniform SceneLighting {
	vec3 AmbientLight;
	float Padding;
	PointLight PointLights[1];
	DirectionalLight DirectionalLights[1];
	Spotlight Spotlights[1];
	int NumPointLights;
	int NumDirectionalLights;
	int NumSpotlights;
	float Padding2;
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

vec3 CalculateSpotlight(Spotlight light) {
	vec3 lightDir = FragToLight(light.Position);
	float theta = dot(lightDir, -light.Direction);

	if (theta > light.CutoffAngle) {
		return vec3(1,1,1);//GetDiffuse(-light.Direction, light.Color);
	}
	return vec3(0,0,0);
}

vec3 CalculateDirectionalLight(DirectionalLight light) {
	vec3 lightDir = light.Direction; // idk why we can do this (this should be negative)? but it works like this
	vec3 diffuse = GetDiffuse(lightDir, light.Color);

	return diffuse;
}

float CalculateAttenuation(vec3 attenuation, float dist) {
	return 1.0f / (attenuation.x + attenuation.y * dist + attenuation.z * (dist * dist));
}

void main()
{
    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	vec3 light = AmbientLight;

	// Point lights
	for (int i = 0; i < NumPointLights; ++i) {
		// Attenuation
		float dist = distance(PointLights[i].Position, inVert.ViewPosition);
		float attenuation = CalculateAttenuation(PointLights[i].Attenuation, dist);

		// Diffuse
		vec3 diffuse = CalculatePointLight(PointLights[i]) * attenuation;
		light += diffuse;
	}

		// Spotlights
	for (int i = 0; i < NumSpotlights; ++i) {
		// Attenuation
		float dist = distance(Spotlights[i].Position, inVert.ViewPosition);
		float attenuation = CalculateAttenuation(Spotlights[i].Attenuation, dist);

		// Diffuse
		vec3 diffuse = CalculateSpotlight(Spotlights[i]) * attenuation;
		light += diffuse;
	}

	// Directional lights
	for (int i = 0; i < NumDirectionalLights; ++i) {
		// Diffuse
		vec3 diffuse = CalculateDirectionalLight(DirectionalLights[i]);
		light += diffuse;
	}

	FragColor.rgb *= light;
}
