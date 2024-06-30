#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_s.h>
#include <learnopengl/camera.h>
#include <iostream>
#include <deque>

#include "scene_data.cpp";
#include "textures.cpp";
#include "bounds.cpp";
#include "lights.cpp";
#include "materials.cpp";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void Jump();

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const unsigned int TARGET_FPS = 30;
const float distanceToGround = 3.0f;

Camera camera(glm::vec3(0.0f, distanceToGround, 7.0f));
bool firstMouse = true;
bool onAir = false;
float verticalVelocity;
float gravity = 45.0f;
float jumpForce = 19.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float rotateSpeed = 3.0f;

int main()
{
    CreateLights();
    CreateMaterials();
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "cse461_hw2_1901042640", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    Shader myShader("my_vertex_shader.vs", "my_fragment_shader.fs");

    CreateBounds();
    BindVertexData();
    CreateTextures();
    
    myShader.use();
    myShader.setInt("texture0", 0);
    myShader.setInt("texture1", 1);

    double lasttime = glfwGetTime();
    float rotationMultiplier = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        myShader.setVec3("directionalLight.direction", directionalLight.direction);
        myShader.setVec3("directionalLight.ambient", directionalLight.ambient);
        myShader.setVec3("directionalLight.diffuse", directionalLight.diffuse);
        myShader.setVec3("directionalLight.specular", directionalLight.specular);

        myShader.setVec3("pointLights[0].position", pointLights[0].position);
        myShader.setVec3("pointLights[0].ambient", pointLights[0].ambient);
        myShader.setVec3("pointLights[0].diffuse", pointLights[0].diffuse);
        myShader.setVec3("pointLights[0].specular", pointLights[0].specular);
        myShader.setFloat("pointLights[0].constant", pointLights[0].constant);
        myShader.setFloat("pointLights[0].linear", pointLights[0].linear);
        myShader.setFloat("pointLights[0].quadratic", pointLights[0].quadratic);


        myShader.setVec3("pointLights[1].position", pointLights[1].position);
        myShader.setVec3("pointLights[1].ambient", pointLights[1].ambient);
        myShader.setVec3("pointLights[1].diffuse", pointLights[1].diffuse);
        myShader.setVec3("pointLights[1].specular", pointLights[1].specular);
        myShader.setFloat("pointLights[1].constant", pointLights[1].constant);
        myShader.setFloat("pointLights[1].linear", pointLights[1].linear);
        myShader.setFloat("pointLights[1].quadratic", pointLights[1].quadratic);

        myShader.setVec3("pointLights[2].position", pointLights[2].position);
        myShader.setVec3("pointLights[2].ambient", pointLights[2].ambient);
        myShader.setVec3("pointLights[2].diffuse", pointLights[2].diffuse);
        myShader.setVec3("pointLights[2].specular", pointLights[2].specular);
        myShader.setFloat("pointLights[2].constant", pointLights[2].constant);
        myShader.setFloat("pointLights[2].linear", pointLights[2].linear);
        myShader.setFloat("pointLights[2].quadratic", pointLights[2].quadratic);

        float currentFrame = static_cast<float>(glfwGetTime());

        /*
        if (currentFrame - lastFrame < 1.0 / TARGET_FPS)
        {
            continue;
        }
        */

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        rotationMultiplier += deltaTime;

        if (onAir)
        {
            Jump();
        }

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        myShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        myShader.setMat4("view", view);

        myShader.setVec3("viewPosition", camera.Position);

        myShader.setVec3("material.ambient", materials[1].ambient);
        myShader.setVec3("material.diffuse", materials[1].diffuse);
        myShader.setVec3("material.specular", materials[1].specular);
        myShader.setFloat("material.shininess", materials[1].shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);

        myShader.setMat4("model", model);

        glBindVertexArray(wallsVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        myShader.setVec3("material.ambient", materials[0].ambient);
        myShader.setVec3("material.diffuse", materials[0].diffuse);
        myShader.setVec3("material.specular", materials[0].specular);
        myShader.setFloat("material.shininess", materials[0].shininess);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ceilingTexture);

        model = glm::mat4(1.0f);

        myShader.setMat4("model", model);

        glBindVertexArray(ceilingVAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        myShader.setMat4("model", model);

        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glBindVertexArray(cubeVAO);
        model = glm::mat4(1.0f);

        myShader.setVec3("material.ambient", materials[2].ambient);
        myShader.setVec3("material.diffuse", materials[2].diffuse);
        myShader.setVec3("material.specular", materials[2].specular);
        myShader.setFloat("material.shininess", materials[2].shininess);
       
        model = glm::translate(model, glm::vec3(-15.0f, 1.0f, -15.0f));
        model = glm::scale(model, glm::vec3(1.0f, 6.0f, 1.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rainbowTexture);

        myShader.setBool("blend", true);
        model = glm::translate(model, glm::vec3(14.0f, 2.0f, 14.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(-15.0f, 8.0f, -15.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, (float)(rotationMultiplier * rotateSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(14.0f, 4.0f, 6.0f));
        model = glm::rotate(model, (float)(rotationMultiplier * rotateSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
        myShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setBool("blend", false);

        myShader.setVec3("material.ambient", materials[0].ambient);
        myShader.setVec3("material.diffuse", materials[0].diffuse);
        myShader.setVec3("material.specular", materials[0].specular);
        myShader.setFloat("material.shininess", materials[0].shininess);

        model = glm::translate(model, glm::vec3(14.0f, 7.0f, 14.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, .5f, glm::vec3(0.0f, 0.0f, 1.0f));
        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setBool("blend", false);
        myShader.setVec3("material.ambient", materials[3].ambient);
        myShader.setVec3("material.diffuse", materials[3].diffuse);
        myShader.setVec3("material.specular", materials[3].specular);
        myShader.setFloat("material.shininess", materials[3].shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rockTexture);
        glBindVertexArray(shapeFVAO);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
        myShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setVec3("material.ambient", materials[4].ambient);
        myShader.setVec3("material.diffuse", materials[4].diffuse);
        myShader.setVec3("material.specular", materials[4].specular);
        myShader.setFloat("material.shininess", materials[4].shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sandTexture);
        glBindVertexArray(pyramidVAO);

        model = glm::translate(model, glm::vec3(-13.0f, 1.0f, 13.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(-13.0f, 1.0f, 8.5f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setBool("blend", true);

        model = glm::translate(model, glm::vec3(-13.0f, 6.0f, 8.5f));
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.75f));

        myShader.setVec3("material.ambient", materials[5].ambient);
        myShader.setVec3("material.diffuse", materials[5].diffuse);
        myShader.setVec3("material.specular", materials[5].specular);
        myShader.setFloat("material.shininess", materials[5].shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        glBindVertexArray(pyramidVAO);

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setVec3("material.ambient", materials[1].ambient);
        myShader.setVec3("material.diffuse", materials[1].diffuse);
        myShader.setVec3("material.specular", materials[1].specular);
        myShader.setFloat("material.shininess", materials[1].shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rockTexture);
        glBindVertexArray(pyramidVAO);

        model = glm::translate(model, glm::vec3(-13.0f, 7.0f, 13.0f));
        model = glm::scale(model, glm::vec3(1.5f, 0.5f, 1.75f));
        model = glm::rotate(model, (float)(-rotationMultiplier * rotateSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

        myShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        myShader.setBool("blend", false);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, goldTexture);
        glBindVertexArray(halfCubeVAO);

        myShader.setVec3("material.ambient", materials[5].ambient);
        myShader.setVec3("material.diffuse", materials[5].diffuse);
        myShader.setVec3("material.specular", materials[5].specular);
        myShader.setFloat("material.shininess", materials[5].shininess);

        model = glm::translate(model, glm::vec3(15.0f, 2.0f, -15.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        myShader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);

        std::cout << "FPS: " << (1 / deltaTime) << " - Delta Time: " << deltaTime << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ReleaseVertexData();
    glfwTerminate();
    return 0;
}

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

void Jump()
{
    verticalVelocity = verticalVelocity - gravity * deltaTime;
    camera.Position += glm::vec3(0, 1, 0) * verticalVelocity * deltaTime;
    if (camera.Position.y <= distanceToGround)
    {
        camera.Position.y = distanceToGround;
        verticalVelocity = 0.0f;
        onAir = false;
    }
}