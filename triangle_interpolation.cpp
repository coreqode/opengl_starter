
#include <stdio.h>
#include <string.h>
#include <iostream>
//GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

static const char *vShader = "                                      \n\
#version 330 core                                                 \n\
                                                                    \n\
layout (location = 0) in vec3 aPos;                                  \n\
layout (location = 1) in vec3 aColor;                                \n\
out vec3 ourColor;                                                     \n\
void main(){                                                        \n\
    gl_Position = vec4(aPos, 1.0);                                   \n\
    ourColor = aColor;                                                 \n\
}\0";

static const char *fShader = "                                      \n\
#version 330 core                                                \n\
out vec4 FragColor;                                                        \n\
in vec3 ourColor;                                               \n\
                                                                    \n\
void main(){                                                        \n\
    FragColor = vec4(ourColor, 1.0);                                  \n\
}\0";

void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result){
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return ;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders(){
    shader = glCreateProgram();

    if (!shader){
        printf("Error creating shader program");
        return ;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (! result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (! result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
    }
}

void CreateTriangle(){
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
    
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

int main(){
    //Initialise the GLFW
    if ( !glfwInit() ){
        printf("GLFW initialisation failed");
        glfwTerminate();
        return 1;
    }
    
    //setup the GLFW window properties
    // OpenGl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow){
        printf("GLFLW window creation is failed");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferwidth, bufferheight;
    glfwGetFramebufferSize(mainWindow, &bufferwidth, &bufferheight);

    //Set contxt for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK){
        printf("GLEW initialization failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //Setup viewport size
    glViewport(0, 0, bufferwidth, bufferheight);
    
    CreateTriangle();
    CompileShaders();

    //Loop until window is closed
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
            glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(mainWindow);

    }
    glBindVertexArray(0);
    glUseProgram(0);

    return 0;
}
