struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

DirectionalLight directionalLight;
PointLight pointLights[3];

void CreateLights()
{
    directionalLight.direction = glm::vec3(-0.7f, -1.0f, -0.3f);
    directionalLight.ambient = glm::vec3(0.15f, 0.15f, 0.15f);
    directionalLight.diffuse = glm::vec3(0.15f, 0.15f, 0.15f);
    directionalLight.specular = glm::vec3(0.15f, 0.15f, 0.15f);

    pointLights[0].position = glm::vec3(-9, 5, 10);
    pointLights[0].ambient = glm::vec3(0.0f, 1.0f, 0.1f);
    pointLights[0].diffuse = glm::vec3(0.0f, 1.0f, 0.1f);
    pointLights[0].specular = glm::vec3(0.0f, 1.0f, 0.1f);
    pointLights[0].constant = 1.0f;
    pointLights[0].linear = 0.05f;
    pointLights[0].quadratic = 0.025f;

    pointLights[1].position = glm::vec3(9, 5, 9);
    pointLights[1].ambient = glm::vec3(0.1f, 0.1f, 0.99f);
    pointLights[1].diffuse = glm::vec3(0.1f, 0.1f, 0.99f);
    pointLights[1].specular = glm::vec3(0.8f, 0.8f, 0.99f);
    pointLights[1].constant = 1.0f;
    pointLights[1].linear = 0.09f;
    pointLights[1].quadratic = 0.032f;
    
    pointLights[2].position = glm::vec3(0, 3, -7);
    pointLights[2].ambient = glm::vec3(0.8f, 0.0, 0.5f);
    pointLights[2].diffuse = glm::vec3(0.8f, 0.0f, 0.5f);
    pointLights[2].specular = glm::vec3(0.8f, 0.0f, 0.5f);
    pointLights[2].constant = 1.0f;
    pointLights[2].linear = 0.001f;
    pointLights[2].quadratic = 0.001f;
}