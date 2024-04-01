#version 430 core
out vec4 FragColor;
	
struct Material{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    bool HasSpecular;
    sampler2D normal;
    bool HasNormal;
    float shininess;
};
struct PointLight{
    vec3 ambient;
    vec3 diffuse;
    vec3 pos;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct Light{
    vec3 diffuse;
    vec3 direction;
    vec3 specular;
};


in vec2 TexCoords;
in vec3 aNormal;
in vec3 aFragPos;
in vec4 FragPosLightSpace;
in mat3 TBN;

vec3 Normal;
vec3 FragPos;
vec3 viewPos;
//! CHANGE MATRIX CALCULATION FROM FS TO VS.
uniform Material mat;
uniform Light lights[16]; 
uniform PointLight Pointlights[16];
uniform int LightCount;
uniform int PointLightCount;
uniform vec3 AviewPos;
uniform sampler2D shadowMap;
uniform samplerCube PointLightShadowMap;
uniform float FarPlane;
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
 



float ShadowCacluclation(vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005); 
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
float PointLightShadowCalc(vec3 FPos)
{
    vec3 lightPos = Pointlights[0].pos;
    vec3 fragToLight = (FPos - TBN * lightPos);
    if(length(fragToLight) > FarPlane - 3.0)
        return 0.0;
    float closestDepth = texture(PointLightShadowMap, fragToLight).r;
    closestDepth *= FarPlane;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(viewPos - FPos);
    float diskRadius = (1.0 + (viewDistance / FarPlane)) / 25.0;  
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(PointLightShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= FarPlane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

vec3 CalculateDiffuse(vec3 LightDiffuse, vec3 lightDir){
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = texture(mat.diffuse, TexCoords).rgb * diff * LightDiffuse;
    return diffuse;
}
vec3 CalculateSpecular(vec3 LightSpecular, vec3 lightDir){
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specularIntensity = vec3(0.5);
    if(mat.HasSpecular){
        specularIntensity = texture(mat.specular, TexCoords).rgb;
    }
    vec3 specular = specularIntensity * spec * LightSpecular;
    return specular;
}
vec3 CalculatePointDiffuse(PointLight light){
    vec3 diffuse = CalculateDiffuse(light.diffuse, normalize(TBN * light.pos - FragPos));
    float distance = length(TBN * light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    return diffuse * attenuation;
}
vec3 CalculatePointSpecular(PointLight light){
    vec3 specular = CalculateSpecular(light.specular, normalize(TBN * light.pos - FragPos));
    float distance = length(TBN * light.pos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    return specular * attenuation;
}


void main()
{         
    FragPos = aFragPos;
    viewPos = AviewPos; 
    vec3 ambient = mat.ambient * texture(mat.diffuse, TexCoords).rgb;
    FragPos = TBN * aFragPos;
    viewPos = TBN * AviewPos;
    if(mat.HasNormal){
        Normal = texture(mat.normal, TexCoords).rgb;
        Normal = normalize(Normal * 2.0 - 1.0);
    }else{
        Normal = aNormal;
    }
    vec3 lighting;
    for(int i = 1; i < LightCount; i++){
        vec3 Ldiffuse = CalculateDiffuse(lights[i].diffuse, -normalize(lights[i].direction));
        vec3 Lspecular = CalculateSpecular(lights[i].specular, -normalize(lights[i].direction));
        lighting += ((Ldiffuse + Lspecular));
    }
    for(int i = 1; i < PointLightCount; i++){
        vec3 Ldiffuse = CalculatePointDiffuse(Pointlights[i]);
        vec3 Lspecular = CalculatePointSpecular(Pointlights[i]);
        lighting += ((Ldiffuse + Lspecular));
    }
    float shadow = ShadowCacluclation(FragPosLightSpace, -normalize(lights[0].direction));
    float PLshadow = PointLightShadowCalc(FragPos);
    vec3 Ldiffuse = CalculatePointDiffuse(Pointlights[0]);
    vec3 Lspecular = CalculatePointSpecular(Pointlights[0]);
    vec3 diffuse = CalculateDiffuse(lights[0].diffuse, -normalize(lights[0].direction));
    vec3 specular = CalculateSpecular(lights[0].specular, -normalize(lights[0].direction));
    vec3 L1lighting = (ambient + (1.0 - (shadow)) * (diffuse + specular));
    vec3 P1lighting = (ambient + (1.0 - (PLshadow)) * (Ldiffuse + Lspecular));
    lighting += L1lighting + P1lighting;
    FragColor = vec4(lighting, 1.0);
}