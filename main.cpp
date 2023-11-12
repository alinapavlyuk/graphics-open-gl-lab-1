#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

void init();
GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* name);
unsigned int compileVertexShader(const char* source);
unsigned int compileFragmentShader(const char* source);
unsigned int compileShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
void setVao(unsigned int &VAO);
void setVbo(unsigned int &VBO, float* vertices, size_t size);
void setEbo(unsigned int &EBO, unsigned int* indices, size_t size);
void handleScale(GLFWwindow* window, unsigned int shaderProgram);
void handleTransform(glm::mat4 matrix, unsigned int shaderProgram);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, glm::mat4 &transform);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "uniform vec3 scale;\n"
                                 "uniform mat4 transform;\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = transform * vec4(aPos.x*scale.x, aPos.y*scale.y, aPos.z*scale.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSourceFigure = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(2.0f/255.0f, 153.0f/255.0f, 29.0f/255.0f, 1.0f);\n"
                                   "}\n\0";

const char *fragmentShaderSourceOutline = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f);\n"
                                   "}\n\0";

int main()
{
    init();
    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 1");
    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = compileVertexShader(vertexShaderSource);

    unsigned int fragmentShader1 = compileFragmentShader(fragmentShaderSourceFigure);
    unsigned int fragmentShader2 = compileFragmentShader(fragmentShaderSourceOutline);

    unsigned int shaderProgram1 = compileShaderProgram(vertexShader, fragmentShader1);
    unsigned int shaderProgram2 = compileShaderProgram(vertexShader, fragmentShader2);

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.25f, -0.25f, 0.0f,
            -0.25f, -0.25f, 0.0f,
            0.7f, 0.2f, 0.0f,
            0.55f, 0.25f, 0.0f,
            0.75f, 0.4f, 0.0f,
            -0.5f, -0.15f, 0.0f,
            -0.5f, 0.15f, 0.0f,
            -0.8f, -0.15f, 0.0f,
            -0.8f, 0.15f, 0.0f,
            -0.5f, 0.45f, 0.0f,
            -0.8f, 0.55f, 0.0f,
            -0.6286f, 0.3214f, 0.0f
    };

    unsigned int indicesFigure[] = {
            0, 1, 2,
            2, 3, 4,
            5, 6, 2,
            3, 7, 8,
            7, 8, 9,
            10, 11, 12,
            11, 12, 13,
            16, 13, 15,
            11, 13, 14
    };

    unsigned int indicesOutline[] = {
            8, 9, 7, 3, 8, 3, 4, 5, 6, 0, 1, 3, 2, 5, 6, 2, 1, 10, 12, 13, 11, 10, 11, 14, 13, 15, 16
    };

    unsigned int VAO1, VAO2, VBO, EBO1, EBO2;
    setVao(VAO1);
    setVbo(VBO, vertices, sizeof(vertices));
    setEbo(EBO1, indicesFigure, sizeof(indicesFigure));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    setVao(VAO2);
    setVbo(VBO, vertices, sizeof(vertices));
    setEbo(EBO2, indicesOutline, sizeof(indicesOutline));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 transform = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, transform);

        glClearColor(228.0f/255.0f, 233.0f/255.0f, 69.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, nullptr);

        handleScale(window, shaderProgram1);
        handleTransform(transform, shaderProgram1);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawElements(GL_LINE_STRIP, 27, GL_UNSIGNED_INT, nullptr);

        handleScale(window, shaderProgram2);
        handleTransform(transform, shaderProgram2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO1);
    glDeleteBuffers(1, &EBO2);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}

void init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow* createWindow(unsigned int width, unsigned int height, const char* name)
{
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}

unsigned int compileVertexShader(const char* source)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return vertexShader;
}

unsigned int compileFragmentShader(const char* source)
{
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, nullptr);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragmentShader;
}

unsigned int compileShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void setVao(unsigned int &VAO) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void setVbo(unsigned int &VBO, float* vertices, size_t size) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
}

void setEbo(unsigned int &EBO, unsigned int* indices, size_t size) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void handleScale(GLFWwindow* window, unsigned int shaderProgram) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float scale = float(height)/float(width);
    int vertexScaleLocation = glGetUniformLocation(shaderProgram, "scale");
    glUniform3f(vertexScaleLocation, scale, 1, 1);
}

void handleTransform(glm::mat4 matrix, unsigned int shaderProgram) {
    int vertexTransformLocation = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(vertexTransformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void processInput(GLFWwindow *window, glm::mat4 &transform)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0.0f, 0.005f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(-0.005f, 0.00f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0.0f, -0.005f, 0.0f));
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0.005f, 0.00f, 0.0f));
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}