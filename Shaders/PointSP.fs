#version 430
in vec4 FragPos;
out vec4 FragColor;
uniform vec3 LightPos;
uniform float FarPlane;

void main()
{
    float LightDistance = length(FragPos.xyz - LightPos);

    LightDistance = LightDistance / FarPlane;
    gl_FragDepth = LightDistance;
}