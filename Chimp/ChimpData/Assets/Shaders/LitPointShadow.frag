#version 410 core

in vec4 FragPos;

layout(std140) uniform PointLightFS {
    vec3 position;
    float farPlane;
};

void main()
{
    // get distance between fragment and light source
    gl_FragDepth = length(FragPos.xyz - position) / farPlane;
}  