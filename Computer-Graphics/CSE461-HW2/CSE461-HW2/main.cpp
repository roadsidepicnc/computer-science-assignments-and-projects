#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <iostream>
#include <deque>
#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void DoJump();
bool CheckCollisionMain(glm::vec3 position);

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

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Borders
{
    glm::vec3 max;
    glm::vec3 min;
};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float verticalVelocity;
float gravity = 0.0085f;
float jumpForce = 0.275f;
float distanceToGround = 2.0f;
bool onAir = false;
Camera camera(glm::vec3(0.0f, distanceToGround, 0.0f));
std::deque <Borders> borderList;

float lastFrame = 0.0f;
float deltaTime = 0.0f;
float currentFrame = 0.0f;
bool firstFrame = true;
glm::vec2 lastFramePos;

#pragma region VERTICES AND INDICES

float floorVertices[] = 
{
    15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    15.0f, 0.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -15.0f, 0.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -15.0f, 0.0f, 15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int floorIndices[] =
{
   0, 1, 2, 
   2, 3, 1
};

float wallVertices[] = 
{
    15.0f, 0.0f, 15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    15.0f, 8.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -15.0f, 0.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -15.0f, 8.0f, 15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    15.0f, 8.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    15.0f, 0.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    15.0f, 8.0f, 15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    15.0f, 8.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -15.0f, 0.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -15.0f, 8.0f, -15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    -15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -15.0f, 8.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -15.0f, 0.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -15.0f, 8.0f, 15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int wallIndices[] =
{
    0, 1, 2,
    2, 3, 1,
    4, 5, 6,
    6, 7, 5,
    8, 9, 10,
    10, 11, 9,
    12, 13, 14,
    14, 15, 13
};

float ceilingVertices[] =
{
    15.0f, 8.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    15.0f, 8.0f, 15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -15.0f, 8.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -15.0f, 8.0f, 15.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int ceilingIndices[] =
{
    0, 1, 2,
    2, 3, 1
};

float cubeVertices[] = 
{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

#pragma endregion

int main()
{
    camera.MovementSpeed = 10.0f;

    float offset = .2;
    Borders currentBorders;
    currentBorders.max.x = 15 + offset;
    currentBorders.min.x = -15 - offset;
    currentBorders.max.z = 16 + offset;
    currentBorders.min.z = 14 - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.x = 15 + offset;
    currentBorders.min.x = -15 - offset;
    currentBorders.max.z = -14 + offset;
    currentBorders.min.z = -16 - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.z = 15 + offset;
    currentBorders.min.z = -15 - offset;
    currentBorders.max.x = -14 + offset;
    currentBorders.min.x = -16 - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.z = 15 + offset;
    currentBorders.min.z = -15 - offset;
    currentBorders.max.x = 16 + offset;
    currentBorders.min.x = 14 - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.z = 11.5f + offset;
    currentBorders.min.z = 8.5f - offset;
    currentBorders.max.x = 11.5f + offset;
    currentBorders.min.x = 8.5f - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.z = 13.0f + offset;
    currentBorders.min.z = 7.0f - offset;
    currentBorders.max.x = -7.0f + offset;
    currentBorders.min.x = -13.0f - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.z = 12.5f + offset;
    currentBorders.min.z = 7.5f - offset;
    currentBorders.max.x = -7.5f + offset;
    currentBorders.min.x = -12.5f - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.x = 12.5f + offset;
    currentBorders.min.x = 7.5f - offset;
    currentBorders.max.z = -7.5f + offset;
    currentBorders.min.z = -12.5f - offset;
    borderList.push_back(currentBorders);

    currentBorders.max.x = -7.5f + offset;
    currentBorders.min.x = -12.5f - offset;
    currentBorders.max.z = -7.5f + offset;
    currentBorders.min.z = -12.5f - offset;
    borderList.push_back(currentBorders);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DOOM", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_WIDTH);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    Shader shader("6.multiple_lights.vs", "6.multiple_lights.fs");
    int width, height, nrChannels;
    unsigned char* data;

#pragma region FLOOR

    unsigned int floorTexture;
    glGenTextures(1, &floorTexture);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("Resources/Floor.jpg", &width, &height, &nrChannels, 0);
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture";
    }

    stbi_image_free(data);

    unsigned int floorVBO, floorVAO, floorEBO;
    glGenVertexArrays(1, &floorVAO);
    glBindVertexArray(floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);

    glGenBuffers(1, &floorEBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

#pragma endregion

#pragma region WALL

    unsigned int wallTexture;
    glGenTextures(1, &wallTexture);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    width, height, nrChannels;
    data = stbi_load("Resources/Wall.png", &width, &height, &nrChannels, 0);
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture";
    }
       
    stbi_image_free(data);

    unsigned int wallVAO, wallVBO, wallEBO;
    glGenVertexArrays(1, &wallVAO);
    glBindVertexArray(wallVAO);

    glGenBuffers(1, &wallVBO);
    glGenBuffers(1, &wallEBO);

    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

#pragma endregion

#pragma region CUBE

    unsigned int cubeTexture, emojiTexture;

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    width, height, nrChannels;
    data = stbi_load("Resources/Cube.png", &width, &height, &nrChannels, 0);
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture";
    }

    stbi_image_free(data); 

    glGenTextures(1, &emojiTexture);
    glBindTexture(GL_TEXTURE_2D, emojiTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    width, height, nrChannels;
    data = stbi_load("Resources/Emoji.jpg", &width, &height, &nrChannels, 0);
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture";
    }

    stbi_image_free(data);

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

#pragma endregion

#pragma region CEILING

    unsigned int ceilingTexture;
    glGenTextures(1, &ceilingTexture);
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("Resources/Ceiling.jpg", &width, &height, &nrChannels, 0);
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture";
    }

    stbi_image_free(data);

    unsigned int ceilingVAO, ceilingVBO, ceilingEBO;
    glGenVertexArrays(1, &ceilingVAO);
    glBindVertexArray(ceilingVAO);

    glGenBuffers(1, &ceilingVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ceilingVBO);

    glGenBuffers(1, &ceilingEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ceilingEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ceilingIndices), ceilingIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

#pragma endregion

#pragma region LIGHT

    DirectionalLight directionalLight;
    directionalLight.direction = glm::vec3(0.0f, 0.0f, 1.0f);
    directionalLight.ambient = glm::vec3(0.5f, 0.0f, 0.0f);
    directionalLight.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    directionalLight.specular = glm::vec3(0.0f, 0.0f, 0.0f);

    PointLight pointLights[3];

    pointLights[0].position = glm::vec3(-5, 5, 10);
    pointLights[0].ambient = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLights[0].diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLights[0].constant = 1.0f;
    pointLights[0].linear = 0.09f;
    pointLights[0].quadratic = 0.032f;

    pointLights[1].position = glm::vec3(5, 5, 10);
    pointLights[1].ambient = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLights[1].diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLights[1].specular = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLights[1].constant = 1.0f;
    pointLights[1].linear = 0.09f;
    pointLights[1].quadratic = 0.032f;

    pointLights[2].position = glm::vec3(-10, 5, 5);
    pointLights[2].ambient = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLights[2].diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLights[2].specular = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLights[2].constant = 1.0f;
    pointLights[2].linear = 0.09f;
    pointLights[2].quadratic = 0.032f;

#pragma endregion

#pragma region MATERIAL

    Material materials[5];
    materials[0].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    materials[0].diffuse = glm::vec3(0.31f, 0.5f, 0.31f);
    materials[0].specular = glm::vec3(0.5f, 0.5f, 0.5f);
    materials[0].shininess = 256.0f;

    materials[1].ambient = glm::vec3(0.85f, 0.2f, 0.75f);
    materials[1].diffuse = glm::vec3(0.64f, 0.7f, 0.9f);
    materials[1].specular = glm::vec3(0.8f, 0.7f, 0.6f);
    materials[1].shininess = 256.0f;

    materials[2].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    materials[2].diffuse = glm::vec3(0.21f, 0.3f, 0.15f);
    materials[2].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    materials[2].shininess = 128.0f;

    materials[3].ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    materials[3].diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    materials[3].specular = glm::vec3(1.0f, 0.0f, 0.0f);
    materials[3].shininess = 16.0f;

    materials[4].ambient = glm::vec3(0.69f, 0.31f, 65.0f);
    materials[4].diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
    materials[4].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    materials[4].shininess = 16.0f;

    Material* currentMaterial;

#pragma endregion

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader.use();
    shader.setInt("FloorTexture", 0);
    shader.setInt("WallTexture", 1);
    shader.setInt("CeilingTexture", 2);
    shader.setInt("CubeTexture", 3);
    shader.setInt("EmojiTexture", 4);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (onAir)
        {
            DoJump();
        }

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        shader.setVec3("directionalLight.direction", directionalLight.direction);
        shader.setVec3("directionalLight.ambient", directionalLight.ambient);
        shader.setVec3("directionalLight.diffuse", directionalLight.diffuse);
        shader.setVec3("directionalLight.specular", directionalLight.specular);

        shader.setVec3("pointLights[0].position", pointLights[0].position);
        shader.setVec3("pointLights[0].ambient", pointLights[0].ambient);
        shader.setVec3("pointLights[0].diffuse", pointLights[0].diffuse);
        shader.setVec3("pointLights[0].specular", pointLights[0].specular);
        shader.setFloat("pointLights[0].constant", pointLights[0].constant);
        shader.setFloat("pointLights[0].linear", pointLights[0].linear);
        shader.setFloat("pointLights[0].quadratic", pointLights[0].quadratic);

        shader.setVec3("pointLights[1].position", pointLights[1].position);
        shader.setVec3("pointLights[1].ambient", pointLights[1].ambient);
        shader.setVec3("pointLights[1].diffuse", pointLights[1].diffuse);
        shader.setVec3("pointLights[1].specular", pointLights[1].specular);
        shader.setFloat("pointLights[1].constant", pointLights[1].constant);
        shader.setFloat("pointLights[1].linear", pointLights[1].linear);
        shader.setFloat("pointLights[1].quadratic", pointLights[1].quadratic);

        shader.setVec3("pointLights[2].position", pointLights[2].position);
        shader.setVec3("pointLights[2].ambient", pointLights[2].ambient);
        shader.setVec3("pointLights[2].diffuse", pointLights[2].diffuse);
        shader.setVec3("pointLights[2].specular", pointLights[2].specular);
        shader.setFloat("pointLights[2].constant", pointLights[2].constant);
        shader.setFloat("pointLights[2].linear", pointLights[2].linear);
        shader.setFloat("pointLights[2].quadratic", pointLights[2].quadratic);

        shader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", camera.GetViewMatrix());

        currentMaterial = &materials[0];
        shader.setVec3("material.ambient", currentMaterial->ambient);
        shader.setVec3("material.diffuse", currentMaterial->diffuse);
        shader.setVec3("material.specular", currentMaterial->specular);
        shader.setFloat("material.shininess", currentMaterial->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(glm::mat4(1.0f)))));
        glBindVertexArray(floorVAO);
        shader.setInt("currentTexture", 0);
        glDrawElements(GL_TRIANGLES, sizeof(floorIndices) / sizeof(floorIndices[1]), GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(glm::mat4(1.0f)))));
        glBindVertexArray(wallVAO);
        shader.setInt("currentTexture", 1);
        glDrawElements(GL_TRIANGLES, sizeof(wallIndices) / sizeof(wallIndices[1]), GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ceilingTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(glm::mat4(1.0f)))));
        glBindVertexArray(ceilingVAO);
        shader.setInt("currentTexture", 2);
        glDrawElements(GL_TRIANGLES, sizeof(ceilingIndices) / sizeof(ceilingIndices[1]), GL_UNSIGNED_INT, 0);
       
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, emojiTexture);
        glBindVertexArray(cubeVAO);
        glm::mat4 cubeModel = glm::mat4(1.0f);
        shader.setInt("currentTexture", 3);

        currentMaterial = &materials[4];
        shader.setVec3("material.ambient", currentMaterial->ambient);
        shader.setVec3("material.diffuse", currentMaterial->diffuse);
        shader.setVec3("material.specular", currentMaterial->specular);
        shader.setFloat("material.shininess", currentMaterial->shininess);
        cubeModel = glm::translate(cubeModel, glm::vec3(-10.0f, 2.0f, -10.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(2.5f, 1.25f, 2.5f));
        shader.setMat4("model", cubeModel);
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(cubeModel))));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cubeModel = glm::mat4(1.0f);
        currentMaterial = &materials[1];
        shader.setVec3("material.ambient", currentMaterial->ambient);
        shader.setVec3("material.diffuse", currentMaterial->diffuse);
        shader.setVec3("material.specular", currentMaterial->specular);
        shader.setFloat("material.shininess", currentMaterial->shininess);
        cubeModel = glm::translate(cubeModel, glm::vec3(-10.0f, 2.0f, 10.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(3.0f, 1.5f, 3.0f));
        shader.setMat4("model", cubeModel);
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(cubeModel))));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cubeModel = glm::mat4(1.0f);
        currentMaterial = &materials[2];
        shader.setVec3("material.ambient", currentMaterial->ambient);
        shader.setVec3("material.diffuse", currentMaterial->diffuse);
        shader.setVec3("material.specular", currentMaterial->specular);
        shader.setFloat("material.shininess", currentMaterial->shininess);
        cubeModel = glm::translate(cubeModel, glm::vec3(10.0f, 2.0f, -10.0f));
        cubeModel = glm::rotate(cubeModel, (float)glfwGetTime() * 1.25f, glm::vec3(0.0f, 1.0f, 0.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(2.5f, 1.25f, 2.5f));
        shader.setMat4("model", cubeModel);
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(cubeModel))));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cubeModel = glm::mat4(1.0f);
        currentMaterial = &materials[3];
        shader.setVec3("material.ambient", currentMaterial->ambient);
        shader.setVec3("material.diffuse", currentMaterial->diffuse);
        shader.setVec3("material.specular", currentMaterial->specular);
        shader.setFloat("material.shininess", currentMaterial->shininess);
        cubeModel = glm::translate(cubeModel, glm::vec3(10.0f, 2.0f, 10.0f));
        cubeModel = glm::scale(cubeModel, glm::vec3(1.5f, 0.75f, 1.5f));
        shader.setMat4("model", cubeModel);
        shader.setMat3("normalModel", glm::mat3(glm::transpose(glm::inverse(cubeModel))));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.setVec3("viewPos", camera.Position); 

        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#pragma region CAMERA

void processInput(GLFWwindow* window)
{
    glm::vec3 updatedPosition;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {  
        updatedPosition = camera.Position + camera.MovementSpeed * deltaTime * glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
        if (!CheckCollisionMain(updatedPosition))
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        updatedPosition = camera.Position - camera.MovementSpeed * deltaTime * glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
        if (!CheckCollisionMain(updatedPosition))
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        updatedPosition = camera.Position - camera.MovementSpeed * deltaTime * glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z));
        if (!CheckCollisionMain(updatedPosition))
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        updatedPosition = camera.Position + camera.MovementSpeed * deltaTime * glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z));
        if (!CheckCollisionMain(updatedPosition))
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !onAir)
    {
        onAir = true;
        verticalVelocity = verticalVelocity + jumpForce;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void DoJump()
{
    verticalVelocity = verticalVelocity - gravity;
    camera.Position.y += verticalVelocity;
    if (camera.Position.y <= distanceToGround)
    {
        camera.Position.y = distanceToGround;
        verticalVelocity = 0.0f;
        onAir = false;
    }
}

bool CheckCollisionHelper(Borders objectBorders, glm::vec3 position)
{
    return(objectBorders.max.x > position.x && objectBorders.min.x < position.x && objectBorders.max.z > position.z && objectBorders.min.z < position.z);
}

bool CheckCollisionMain(glm::vec3 position) 
{
    for (int i = 0; i < borderList.size(); i++) 
    {
        if (CheckCollisionHelper(borderList[i], position))
        {
            return true;
        }
    }

    return false;
}

#pragma endregion