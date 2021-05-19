
#include <stdio.h>
#include <string.h>
#include <iostream>
//GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint EBO, VAO1, VAO2, VBO1, VBO2, shader;

//Vertx Shaders
static const char *vShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
void main(){                                                        \n\
    gl_Position = vec4(0.4* pos.x, 0.4* pos.y, 0.4 * pos.z, 1.0f);                  \n\
}\0";

static const char *fShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 colour;                                                    \n\
                                                                    \n\
void main(){                                                        \n\
    colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);                          \n\
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
        printf("Error linking program:\n", eLog);
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (! result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
    }
}

void CreateRectangle(){
    GLfloat vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top
    };


//    GLint indices[] = {
//        0, 1, 3,
//        1, 2, 3
//    };

    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

        glGenBuffers(1, &VBO1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//            glGenBuffers(1, &EBO);
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

void CreateTriangle(){
    GLfloat vertices1[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    GLfloat vertices2[] = {
        0.5f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.5f, 1.0f, 0.0f};

    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

        glGenBuffers(1, &VBO1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

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
//    CreateRectangle();
    CompileShaders();

    //Loop until window is closed
    while(!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
            glBindVertexArray(VAO1);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(VAO2);
                glDrawArrays(GL_TRIANGLES, 0, 6);
//                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(mainWindow);
        glBindVertexArray(0);
        glUseProgram(0);

    }


    return 0;
}
