#version 330 core

out vec4 FragColor;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirectionalLight 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform bool blend;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[3];

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular; 

    ambient = light.ambient * material.ambient ;
    diffuse = light.diffuse * diff * material.diffuse;
    specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 result;

    result = CalcDirLight(directionalLight, norm, viewDir);
   
    for(int i = 0; i < 3; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    if(blend)
    {
        result *= vec3(mix(texture(texture0, TexCoords), texture(texture1, TexCoords), 0.4));
    }
    else
    {
        result*= vec3(texture(texture0, TexCoords));
    }

    FragColor = vec4(result, 1.0);
}