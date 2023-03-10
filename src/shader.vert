#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec2 TexCoords;
    vec4 FragPosLightSpace;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 PVM;
uniform mat4 model;
uniform mat4 normal_model;
uniform mat4 lightSpaceMatrix;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    vec3 T = normalize(mat3(normal_model) * aTangent);
    vec3 B = normalize(mat3(normal_model) * aBitangent);
    vec3 N = normalize(mat3(normal_model) * aNormal);    

    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * FragPos;

    gl_Position = PVM * vec4(aPos, 1.0);
}   
