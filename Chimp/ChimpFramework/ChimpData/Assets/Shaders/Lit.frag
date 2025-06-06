#version 410 core

in OutputVertex {
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoords;
	vec4 SpotlightPosition[4];
	vec4 DirectionalPosition[4];
} inVert;

out vec4 FragColor;

uniform sampler2D u_ActiveTexture;
uniform sampler2DArray u_ShadowMaps;
uniform samplerCubeArray u_CubeShadowMaps;

struct PointLight {
	vec3 Position;
	float Padding;

	vec3 Color;
	float Padding2;

	vec3 Attenuation;
	float Padding3;

	float FarPlane;
	int CubeRootNumShadowSamples;
	float ShadowBias;
	float ShadowMaxSampleDistance;
};

struct DirectionalLight {
	vec3 Direction; // direction light is pointing
	float Padding;

	vec3 Color;
	float Padding2;

	float ShadowBias;
	int SqrtNumShadowSamples;
	float Padding3;
	float Padding4;
};

struct Spotlight {
	vec3 Direction;
	float Padding;

	vec3 Position;
	float Padding2;

	vec3 Color;
	float Padding3;

	vec3 Attenuation;
		
	float CutoffAngle; // Is Cos(Angle)

	float ShadowBias;
	int SqrtNumShadowSamples;
	float Padding4;
	float Padding5;
};

layout (std140) uniform SceneLighting {
	vec3 AmbientLight;
	float Padding;
	PointLight PointLights[4];
	DirectionalLight DirectionalLights[4];
	Spotlight Spotlights[4];
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

float CalculateRegularShadow(vec4 lightSpacePos, float bias, int sqrtNumSamples, int mapIndex) {
	// Get UV coordinates in the shadow map
	vec3 uvz = lightSpacePos.xyz / lightSpacePos.w;
	uvz *= 0.5f;
	uvz += vec3(0.5f, 0.5f, 0.5f);

	// Fragment is past the far plane of the camera used for depth map
	if (uvz.z > 1) return 1;

	// Is in shadow?
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMaps, 0).xy;
	int startingSample = -(sqrtNumSamples / 2);
	int finalSample = startingSample + sqrtNumSamples;

	for (int x = startingSample; x < finalSample; ++x)
	{
		for (int y = startingSample; y < finalSample; ++y)
		{
			float pcfDepth = texture(u_ShadowMaps, vec3(uvz.xy + vec2(x, y) * texelSize, mapIndex)).r; 
			shadow += uvz.z - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	float totalSamples = sqrtNumSamples * sqrtNumSamples;
	return 1 - shadow / totalSamples;
}

float CalculatePointShadow(PointLight light, vec3 viewFragPos, int mapIndex) {
	vec3 fragToLight = viewFragPos - light.Position;
	float currentDepth = length(fragToLight);
	float shadow = 0.0f;

	int startingSample = -(light.CubeRootNumShadowSamples / 2);
	int finalSample = startingSample + light.CubeRootNumShadowSamples;

	for (int x = startingSample; x < finalSample; x++) {
		for (int y = startingSample; y < finalSample; y++) {
			for (int z = startingSample; z < finalSample; z++) {
				vec3 offset = light.ShadowMaxSampleDistance * vec3(x, y, z);
				float closestDepth = texture(u_CubeShadowMaps, vec4(fragToLight + offset, mapIndex)).r * light.FarPlane;
				if (currentDepth - light.ShadowBias > closestDepth) {
					shadow++;
				}
			}
		}
	}

	float totalSamples = light.CubeRootNumShadowSamples * light.CubeRootNumShadowSamples * light.CubeRootNumShadowSamples;
	return 1 - shadow / totalSamples;
}

void main()
{
	if (IsDepthPass != 0) {
		return;
	}

    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	vec3 light = AmbientLight;

	// Point lights
	for (int i = 0; i < NumPointLights; ++i) {
		float shadow = CalculatePointShadow(PointLights[i], inVert.WorldPosition.xyz, i);

		// Attenuation
		float dist = distance(PointLights[i].Position, inVert.WorldPosition);
		float attenuation = CalculateAttenuation(PointLights[i].Attenuation, dist);

		// Diffuse
		vec3 diffuse = CalculatePointLight(PointLights[i]);
		light += diffuse * attenuation * diffuse * shadow;
	}

	// Spotlights
	for (int i = 0; i < NumSpotlights; ++i) {
		float shadow = CalculateRegularShadow(inVert.SpotlightPosition[i], Spotlights[i].ShadowBias, Spotlights[i].SqrtNumShadowSamples, i + NumDirectionalLights);
		// Attenuation
		float dist = distance(Spotlights[i].Position, inVert.WorldPosition);
		float attenuation = CalculateAttenuation(Spotlights[i].Attenuation, dist);

		// Diffuse
		vec3 diffuse = CalculateSpotlight(Spotlights[i]);

		light +=  diffuse * attenuation * shadow;
	}

	// Directional lights
	for (int i = 0; i < NumDirectionalLights; ++i) {
		float shadow = CalculateRegularShadow(inVert.DirectionalPosition[i], DirectionalLights[i].ShadowBias, DirectionalLights[i].SqrtNumShadowSamples, i);
		// Diffuse
		vec3 diffuse = CalculateDirectionalLight(DirectionalLights[i]);
		light += diffuse * shadow;
		
	}

	FragColor.rgb *= light;
}
