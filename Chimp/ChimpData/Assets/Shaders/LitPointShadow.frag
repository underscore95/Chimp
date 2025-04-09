#version 410 core

in vec3 FragPos;

layout(std140) uniform PointLightFS {
    vec3 position;
    float farPlane;
};

void main()
{
    // get distance between fragment and light source
    gl_FragDepth = length(FragPos.xyz - position.xyz) / farPlane;
}  