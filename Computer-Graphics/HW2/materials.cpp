struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

Material materials[6];

void CreateMaterials()
{
    materials[0].ambient = glm::vec3(0.6f, 0.6f, 0.6f);
    materials[0].diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
    materials[0].specular = glm::vec3(0.6f, 0.0f, 0.6f);
    materials[0].shininess = 300.0f;

    materials[1].ambient = glm::vec3(0.75f, 0.25f, 0.25f);
    materials[1].diffuse = glm::vec3(0.75f, 0.25f, 0.25f);
    materials[1].specular = glm::vec3(0.75f, 0.1f, 0.1f);
    materials[1].shininess = 128.0f;

    materials[2].ambient = glm::vec3(0.2f, 0.6f, 0.5f);
    materials[2].diffuse = glm::vec3(0.2f, 0.6f, 0.5f);
    materials[2].specular = glm::vec3(0.2f, 0.6f, 0.5f);
    materials[2].shininess = 128.0f;

    materials[3].ambient = glm::vec3(0.9f, 0.9f, 0.9f);
    materials[3].diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
    materials[3].specular = glm::vec3(0.9f, 0.0f, 0.9f);
    materials[3].shininess = 256.0f;
    
    materials[4].ambient = glm::vec3(0.75f, 0.95f, 0.75f);
    materials[4].diffuse = glm::vec3(0.65f, 0.65f, 0.65f);
    materials[4].specular = glm::vec3(0.1f, 1.0f, 0.1f);
    materials[4].shininess = 16.0f;
    
    materials[5].ambient = glm::vec3(0.25f, 0.25f, 0.65f);
    materials[5].diffuse = glm::vec3(0.25f, 0.25f, 0.65f);
    materials[5].specular = glm::vec3(0.25f, 0.4f, 0.4f);
    materials[5].shininess = 624.0f;
}