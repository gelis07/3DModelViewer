#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
	
out vec2 TexCoords;
out vec3 aNormal;
out vec3 aFragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;
void main()
{
    TexCoords = aTexCoords;
    aFragPos = vec3(model * vec4(aPos, 1.0));
    aNormal = aNorm;
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNorm, 0.0)));
    TBN = mat3(T,B,N);
    FragPosLightSpace = lightSpaceMatrix * vec4(aFragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}