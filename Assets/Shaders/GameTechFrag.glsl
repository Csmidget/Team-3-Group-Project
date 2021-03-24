#version 400 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float farPlane;

    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    samplerCube shadowMap;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float farPlane;

    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    mat4 shadowVP;
    sampler2D shadowMap;
};

#define NR_POINT_LIGHTS 5

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 vertColour;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight[NR_POINT_LIGHTS];
uniform Material material;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,float shadowed, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 tex);
vec3 CalcSpotLight(SpotLight light, float shadowed, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 tex);

// ������Դ��Ӱ
float ShadowCalculation(vec3 fragPos, PointLight light);
float ShadowCalculation(vec3 fragPos, SpotLight light);

void main()
{    

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 tex = texture(material.diffuse, TexCoords);

    // ���յ���ɫ����
    vec3 result = vec3(0.0f);
    // ������Դ

    /*
    for(int i = 0; i < 1; i++)
    {
        float shadow=ShadowCalculation(FragPos,pointLights[i]);
        shadow=clamp(1.0-shadow,0.0,1.0);
        //float shadow=1.0;
        result += CalcPointLight(pointLights[i],shadow, norm, FragPos, viewDir);  
    }
    */


    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        float shadow=ShadowCalculation(FragPos,pointLights[i]);
        shadow=clamp(1.0-shadow,0.0,1.0);
        //float shadow=1.0;
        result += CalcPointLight(pointLights[i],shadow, norm, FragPos, viewDir, tex);  
    }
  // {
  //      float shadow=ShadowCalculation(FragPos,pointLights[1]);
  //      shadow=clamp(1.0-shadow,0.0,1.0);
  //      //float shadow=1.0;
  //      result += CalcPointLight(pointLights[1],shadow, norm, FragPos, viewDir);  
  //  }
  //      {
  //      float shadow=ShadowCalculation(FragPos,pointLights[3]);
  //      shadow=clamp(1.0-shadow,0.0,1.0);
  //      //float shadow=1.0;
  //      result += CalcPointLight(pointLights[1],shadow, norm, FragPos, viewDir);  
  //  }

    // ����۹��
    for(int i = 0; i < 1; i++)
    {
        //float shadow=ShadowCalculation(FragPos,spotLight[i]);
        //shadow=clamp(1.0-shadow,0.0,1.0);
        float shadow=1.0;
        result += CalcSpotLight(spotLight[i],shadow, norm, FragPos, viewDir, tex);    
    }

    FragColor = vec4(result, 1.0);
}


float linearDepth(float depthSample,float zNear,float zFar)
{
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

float ShadowCalculation(vec3 fragPos, SpotLight light)
{
    vec2 sampleOffsetDirections[9] = vec2[]
    (
       vec2(-1, 1), vec2(0, 1), vec2(1, 1),
       vec2(-1, 0), vec2(0, 0), vec2(1, 0),
       vec2(-1, -1), vec2(0, -0), vec2(1,-0)
    );
 
     vec4 shadowCoord = light.shadowVP*vec4(fragPos,1.0);
    shadowCoord.xyz/=shadowCoord.w;
    shadowCoord.xy=shadowCoord.xy*0.5+0.5;
    vec3 fragToLight = fragPos - light.position;
    float fragDepth = length(fragToLight);
    //float texelSize = (1.0 + (fragDepth / light.farPlane)) / 5000.0;
    float texelSize = 0.0;
    float bias = 0.05;
    float shadow = 0;
    for(int i = 0; i < 9; i++)
    {
        float closestDepth = texture(light.shadowMap, shadowCoord.xy + sampleOffsetDirections[i] * texelSize).r;
        closestDepth *= light.farPlane;
        if(fragDepth - bias > closestDepth)
            shadow += 0.93f;
    }
    shadow /= 9;
    return shadow;
    //return 0.0;
}

float ShadowCalculation(vec3 fragPos, PointLight light)
{
    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
       vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
       vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
       vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
       vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );
 
    vec3 fragToLight = fragPos - light.position;
    float fragDepth = length(fragToLight);
    float texelSize = (1.0 + (fragDepth / light.farPlane)) / 50.0;
    float bias = 0.05;
    float shadow = 0;
    for(int i = 0; i < 20; i++)
    {
        float closestDepth = texture(light.shadowMap, fragToLight + sampleOffsetDirections[i] * texelSize).r;
        closestDepth *= light.farPlane;
        if(fragDepth - bias > closestDepth)
            shadow += 0.93f;
    }
    shadow /= 20;
    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords) * vertColour);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords) * vertColour);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords) * vertColour);
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light,float shadowed, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 tex)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * vec3(tex * vertColour);
    vec3 diffuse = light.diffuse * diff * vec3(tex * vertColour);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords) * vertColour);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular)*shadowed;
}


vec3 CalcSpotLight(SpotLight light,float shadowed, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 tex)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(tex * vertColour);
    vec3 diffuse = light.diffuse * diff * vec3(tex * vertColour);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords) * vertColour);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular)*shadowed;
}