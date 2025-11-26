#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <cmath>

#include <ShaderProgram.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>


#include "../../tp 00/VEtudiant/Fichiers/Color.h"
extern "C" _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //std::cout << xpos << ' ' << ypos << std::endl;

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{/*
    GLboolean* getPosition = reinterpret_cast<GLboolean*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        *getPosition = GL_TRUE;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        *getPosition = GL_FALSE;*/
}
void CalculCursorPointPosition(GLfloat centreX, GLfloat centreY,GLint width, GLint height, GLfloat ray, GLfloat points[6])
{
    points[0] = centreX + (ray) / width ;
    points[1] = 0.0f;

    points[2] = centreX + (ray * 54/60) / width * cos(-160);
    points[3] = centreY + (ray * 54/60) / height * sin(-160);

    points[4] = centreX + (ray * 54/60) / width * cos(160);
    points[5] = centreY + (ray * 54/60) / height * sin(160);
}


int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit(); //Fonction de la bibliothèque GLEW
    glGetString(GL_VERSION); //Fonction de la bibliothèque OpenGL du système

    glewExperimental = GL_TRUE;

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //shader :
    CShader CircleVertex = CShader{ GL_VERTEX_SHADER, std::ifstream{"Circle.vert"} };
    CShader CursorVertex = CShader{ GL_VERTEX_SHADER, std::ifstream{"Cursor.vert"} };
    CShader fragment = CShader{ GL_FRAGMENT_SHADER, std::ifstream{"Fragment.frag"} };

    CShaderProgram shaderProgram[2]{ {CircleVertex, fragment}, {CursorVertex, fragment} };

    GLint width, height;
    glfwGetWindowSize(window, &width, &height);/*
    GLboolean getPosition;
    glfwSetWindowUserPointer(window, &getPosition);*/
    GLint size = std::min(width, height);
    GLint centreX = width / 2, centreY = height / 2;
    GLint ray = size * 70 / 200, thickness = size * 10 / 200;

    GLuint VAO[2];
    GLuint VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    CColor color{};

    //Circle
    const GLuint nbPoint = 360;
    GLfloat circleVertices[(nbPoint + 1) * 5 * 2] = { 0 };
    for (GLuint i = 0; i < sizeof(circleVertices) / sizeof(GLfloat); i = i + 5)
    {
        color.SetHSV(int(i / 5 / 2), 1.0, 1.0);
        GLfloat rotation = int(i / 5 / 2) * M_PI / 180;
        if (int(i / 5) % 2 == 0)
        {
            circleVertices[i] = std::round(((centreX + ((ray - thickness) * cos(rotation))) / width * 2 - 1) * 1000) / 1000;
            circleVertices[i + 1] = std::round(((centreY + ((ray - thickness) * sin(rotation))) / height * 2 - 1) * 1000) / 1000;
        }
        else
        {
            circleVertices[i] = std::round(((centreX + ((ray + thickness) * cos(rotation))) / width * 2 - 1) * 1000) / 1000;
            circleVertices[i + 1] = std::round(((centreY + ((ray + thickness) * sin(rotation))) / height * 2 - 1) * 1000) / 1000;
        }
        circleVertices[i + 2] = color.GetR();
        circleVertices[i + 3] = color.GetG();
        circleVertices[i + 4] = color.GetB();
    }
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)2);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    //Cursor
    GLfloat cursorVertices[6] = { 0 };
    CalculCursorPointPosition(centreX, centreY, width, height, ray - thickness, cursorVertices);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    GLfloat rotation = 0;
    GLint nCursorUniform_center = glGetUniformLocation(shaderProgram[1].GetProgramId(), "center");
    GLint nCursorUniform_rotation = glGetUniformLocation(shaderProgram[1].GetProgramId(), "rotation");



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO[0]);
        shaderProgram[0].Use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (nbPoint + 2) * 2);

        /*glBindVertexArray(VAO[1]);
        shaderProgram[1].Use();
        if (getPosition)
        {
            GLdouble cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            rotation = std::atan2(cursorX - (centreX + 1) / 2 * width, cursorY - (centreY + 1) / 2 * height) - 3.1415 / 2;
            std::cout << rotation << std::endl;
        }
        glUniform2f(nCursorUniform_center, centreX, centreY);
        glUniform1f(nCursorUniform_rotation, rotation);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
