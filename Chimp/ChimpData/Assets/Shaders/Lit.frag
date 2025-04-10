#version 410 core

in OutputVertex {
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoords;
	vec4 SpotlightPosition[1];
	vec4 DirectionalPosition[1];
} inVert;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;
uniform sampler2D u_ShadowMap;
uniform samplerCube u_CubeShadowMap;

struct PointLight {
	vec3 Position;
	float Padding;

	vec3 Color;
	float Padding2;

	vec3 Attenuation;
	float Padding3;

	float FarPlane;
	float Padding4;
	float Padding5;
	float Padding6;
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
	float IsDepthPass;
};

vec3 FragToLight(vec3 lightPos) {
	return normalize(lightPos - inVert.WorldPosition);
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
		return GetDiffuse(-light.Direction, light.Color);
	}
	return vec3(0,0,0);
}

vec3 CalculateDirectionalLight(DirectionalLight light) {
	vec3 lightDir = -light.Direction;
	vec3 diffuse = GetDiffuse(lightDir, light.Color);

	return diffuse;
}

float CalculateAttenuation(vec3 attenuation, float dist) {
	return 1.0f / (attenuation.x + attenuation.y * dist + attenuation.z * (dist * dist));
}

float IsInShadow(vec4 lightSpacePos) {
	// Get UV coordinates in the shadow map
	vec3 uvz = lightSpacePos.xyz / lightSpacePos.w;
	uvz *= 0.5f;
	uvz += vec3(0.5f, 0.5f, 0.5f);

	// Is out of map?
	if (uvz.x < 0 || uvz.x > 1 || uvz.y < 0 || uvz.y > 1) return 0;

	// Is in shadow?
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	float bias = 0.05f;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_ShadowMap, uvz.xy + vec2(x, y) * texelSize).r; 
			shadow += uvz.z - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	return 1 - shadow / 9.0f;
}

bool IsInPointShadow(PointLight light, vec3 viewFragPos) {
	vec3 fragToLight = viewFragPos - light.Position; 
    float closestDepth = texture(u_CubeShadowMap, fragToLight).r;
	closestDepth *= light.FarPlane;
	float currentDepth = length(fragToLight);
	return currentDepth - 0.15f > closestDepth ? true : false; 
}

void main()
{
	if (IsDepthPass != 0) {
		return;
	}

    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	vec3 light = AmbientLight;
	bool TRUE = light.x != 0.33;

	// Point lights
	for (int i = 0; i < NumPointLights; ++i) {
		if (!IsInPointShadow(PointLights[i], inVert.WorldPosition.xyz)) {
			// Attenuation
			float dist = distance(PointLights[i].Position, inVert.WorldPosition);
			float attenuation = CalculateAttenuation(PointLights[i].Attenuation, dist);

			// Diffuse
			vec3 diffuse = CalculatePointLight(PointLights[i]) * attenuation;
			light += diffuse;
		}
	}

	// Spotlights
	for (int i = 0; i < NumSpotlights; ++i) {
		float shadow = IsInShadow(inVert.SpotlightPosition[i]);
		// Attenuation
		float dist = distance(Spotlights[i].Position, inVert.WorldPosition);
		float attenuation = CalculateAttenuation(Spotlights[i].Attenuation, dist);

		// Diffuse
		vec3 diffuse = CalculateSpotlight(Spotlights[i]);

		light +=  diffuse * attenuation * shadow;
	}

	// Directional lights
	for (int i = 0; i < NumDirectionalLights; ++i) {
		float shadow = IsInShadow(inVert.DirectionalPosition[i]);
		// Diffuse
		vec3 diffuse = CalculateDirectionalLight(DirectionalLights[i]);
		light += diffuse * shadow;
		
	}

	FragColor.rgb *= light;
}
