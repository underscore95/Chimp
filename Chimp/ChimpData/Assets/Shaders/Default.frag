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
	vec3 Color;
	float LinearAttenuation;
};

layout (std140) uniform SceneLighting {
	int NumPointLights;
	PointLight PointLights[1];
};

vec3 FragToLight(vec3 lightPos) {
	return normalize(lightPos - inVert.Position);
}

vec3 CalculatePointLight(PointLight light) {
	vec3 lightDir = FragToLight(light.Position);
	vec3 diffuse = max(dot(inVert.Normal, -lightDir), 0.0) * light.Color;

	return diffuse;
}

void main()
{
    FragColor = texture(u_ActiveTexture, inVert.TexCoords);
    
	vec3 light = vec3(0.2,0.2,0.2);
	for (int i = 0; i < NumPointLights; ++i) {
		light += CalculatePointLight(PointLights[i]);
	}

	FragColor.rgb *= light;
}
