#include <stdio.h>
#include <string.h>
#include <cmath>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Movement speed
const float SPEED = 0.01f;

// Global variables for OpenGL
GLuint VAO, VBO, EBO, shader, uniformModel, uniformTexture, uniformTintColor;

// Vertex Shader
static const char* vShaderSrc = R"(
#version 330
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoord;
uniform mat4 model;
void main()
{
    gl_Position = model * vec4(pos, 4.4);
    TexCoord = texCoord;
}
)";

// Fragment Shader
static const char* fShaderSrc = R"(
#version 330
in vec2 TexCoord;
out vec4 color;
uniform sampler2D ourTexture;
uniform vec4 tintColor;
void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    color = texColor * tintColor; // Apply tint color
}
)";

float getRandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

void CreateRectangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
         1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Bottom-right
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // Top-right
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f  // Top-left
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, errorLog);
        return;
    }

    glAttachShader(program, shader);
}

void CompileShaders()
{
    shader = glCreateProgram();
    if (!shader)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, vShaderSrc, GL_VERTEX_SHADER);
    AddShader(shader, fShaderSrc, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking program: '%s'\n", errorLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating program: '%s'\n", errorLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
    uniformTexture = glGetUniformLocation(shader, "ourTexture");
    uniformTintColor = glGetUniformLocation(shader, "tintColor");
}

GLuint LoadTexture(const char* filePath)
{
    GLuint textureID;
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (!data)
    {
        printf("Failed to load texture\n");
        return 0;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return textureID;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization failed!\n");
        return 1;
    }

    // Setup GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL App", nullptr, nullptr);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization failed!\n");
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    // Setup OpenGL objects
    CreateRectangle();
    CompileShaders();

    GLuint texture = LoadTexture("../resources/dvdlogo.png");
    if (texture == 0)
    {
        return -1;
    }

    // Initialize position and direction
    float posX = getRandomFloat(-0.8f, 0.8f);
    float posY = getRandomFloat(-0.8f, 0.8f);
    glm::vec2 direction = glm::normalize(glm::vec2(getRandomFloat(-1.0f, 1.0f), getRandomFloat(-1.0f, 1.0f)));

    glm::vec4 currentTintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white (no tint)

    // Main rendering loop
    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        // Update position
        posX += direction.x * SPEED;
        posY += direction.y * SPEED;

        // Check for boundary collision and reverse direction if needed
        if (posX > 0.8f || posX < -0.8f)
        {
            direction.x = -direction.x;
            posX += direction.x * SPEED;
            currentTintColor = glm::vec4(getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), 1.0f);
        }
        if (posY > 0.8f || posY < -0.8f)
        {
            direction.y = -direction.y;
            posY += direction.y * SPEED;
            currentTintColor = glm::vec4(getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), 1.0f);
        }

        // Update model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posX, posY, 0.0f));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(uniformTexture, 0);
        glUniform4fv(uniformTintColor, 1, glm::value_ptr(currentTintColor));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwTerminate();
    return 0;
}
