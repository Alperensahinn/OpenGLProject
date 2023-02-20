#version 440 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    float shininess;
}; 

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec2 TexCoords;
    vec4 FragPosLightSpace;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D shadowMap;

uniform Light light;  
uniform Material material;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
   
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    

    float bias = max(0 * (1.0 - dot(normal, lightDir)), 0.001);

    
    float shadow = 0.0;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
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


void main()
{   
    vec3 color = texture(material.diffuseMap, fs_in.TexCoords).rgb;
    
    vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  

    // ambient
    vec3 ambient = light.ambient;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularMap, fs_in.TexCoords).rgb;  
    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, normal);
    
    vec3 lighting = (ambient +   (1.0 - shadow) * (diffuse + specular)) * color;
    

    FragColor = vec4(lighting, 1.0); 
    //FragColor = vec4(vec3(1.0f - shadow), 1.0f); //debug 
    //FragColor = vec4(lightDir, 1.0); //debug 
    //FragColor = vec4(normal, 1.0); //debug 
    //FragColor = vec4(specular, 1.0); //debug
}