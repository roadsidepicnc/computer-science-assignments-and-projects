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

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform sampler2D FloorTexture;
uniform sampler2D WallTexture;
uniform sampler2D CeilingTexture;
uniform sampler2D CubeTexture;
uniform sampler2D EmojiTexture;
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[3];
uniform int currentTexture;


// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = directionalLight.ambient  * material.ambient;
    vec3 diffuse = directionalLight.diffuse * diff * material.diffuse ;
    vec3 specular = directionalLight.specular * spec * material.specular ;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient  * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    /*ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;*/
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(norm, viewDir);

    for(int i = 0; i < 3; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    if(currentTexture == 0)
    {
        FragColor = vec4(result * vec3(texture(FloorTexture, TexCoords)), 1.0);
    }
    else if(currentTexture == 1)
    {
        FragColor = vec4(result * vec3(texture(WallTexture, TexCoords)), 1.0);
    }
    else if(currentTexture == 2)
    {
        FragColor = vec4(result * vec3(texture(CeilingTexture, TexCoords)), 1.0);
    }
    else if(currentTexture == 3)
    {
        FragColor = vec4(result * vec3(mix(texture(EmojiTexture, TexCoords), texture(CubeTexture, TexCoords), 0.6)), 1.0);
    }
    else
    {
        FragColor = vec4(result, 1.0f);
    }
} 