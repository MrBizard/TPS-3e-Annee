#include <iostream>
#include <fstream>
#include <sstream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "../VEtudiant/Fichiers/Color.h"

extern "C" _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

void openFileShader(std::string fileName, GLint shader)
{
    std::ifstream in(fileName);
    std::stringstream ss;
    ss << in.rdbuf();
    std::string str = ss.str();

    const GLchar* fragmentShaderSource = str.c_str();
    glShaderSource(shader, 1, &fragmentShaderSource, nullptr);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CColor* pColor = reinterpret_cast<CColor*>(glfwGetWindowUserPointer(window));
    float h = pColor->GetH();

    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
    {
        h -= 1;
        if (h < 0)
            h = 1;
    }
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
    {
        h += 1;
        if (h > 360)
            h = 0;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        GLint modes[1];
        glGetIntegerv(GL_POLYGON_MODE, modes);

        if (modes[0] == GL_LINE)
        {
            modes[0] = GL_FILL;
        }
        else
        {
            modes[0] = GL_LINE;
        }
        glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
    }
    pColor->SetHSV(h, pColor->GetS(), pColor->GetV());
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    CColor* pColor = reinterpret_cast<CColor*>(glfwGetWindowUserPointer(window));
    int width = 0, height = 0;
    glfwGetWindowSize(window, &width, &height);

    pColor->SetHSV(pColor->GetH(), xpos / width, ypos / height);
}

void DrawTop(GLuint VAO[], GLint shaderProgram[])
{
    glUseProgram(shaderProgram[0]);
    glBindVertexArray(VAO[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram[1]);
    glBindVertexArray(VAO[1]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));
}

void DrawUnder(GLuint VAO[], GLint shaderProgram[], GLint modes[])
{
    glUseProgram(shaderProgram[2]);
    glBindVertexArray(VAO[2]);
    glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));    
    
    glUseProgram(shaderProgram[3]);
    glBindVertexArray(VAO[3]);
    glPolygonMode(GL_FRONT_AND_BACK, modes[1]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));

    glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
}
int main(int argc, char* argv[])
{
    GLFWwindow* window;
    CColor* pcolor = new CColor();

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
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

    glfwSetWindowUserPointer(window, pcolor);

    GLfloat vertices[] = {
        //triangle
        -0.75f, 0.75f,
        -0.25f, 0.75f,
        -0.5f, 0.25f
    };

    GLfloat rVertices[] = {
        //Rectangle
        0.25f, 0.625f,
        0.75f, 0.625f,
        0.75f, 0.375f,
        0.25f, 0.375f
    };

    GLfloat r1Vertices[] = {
        //Rectangle 1
        -0.75f, -0.25f,
        0.0f, -0.25f, 
        0.0f, -0.75f, 
        -0.75f, -0.75f
    };

    GLfloat r2Vertices[] = {
        //Rectangle 1
        0.0f, -0.25f,
        0.75f, -0.25f,
        0.75f, -0.75f,
        0.0f, -0.75f,
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };     

    //shader :
    GLint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    openFileShader("Vertex.vert", vertexShader);
    glCompileShader(vertexShader);


    GLint fragmentShader[4];
    fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
    openFileShader("Triangle.frag", fragmentShader[0]);
    glCompileShader(fragmentShader[0]);

    fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
    openFileShader("Rectangle.frag", fragmentShader[1]);
    glCompileShader(fragmentShader[1]);

    fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
    openFileShader("R1Rectangle.frag", fragmentShader[2]);
    glCompileShader(fragmentShader[2]);  

    fragmentShader[3] = glCreateShader(GL_FRAGMENT_SHADER);
    openFileShader("R2Rectangle.frag", fragmentShader[3]);
    glCompileShader(fragmentShader[3]);

    GLint shaderProgram[4];
    shaderProgram[0] = glCreateProgram();
    shaderProgram[1] = glCreateProgram();
    shaderProgram[2] = glCreateProgram();
    shaderProgram[3] = glCreateProgram();

    glAttachShader(shaderProgram[0], vertexShader);
    glAttachShader(shaderProgram[0], fragmentShader[0]);
    glLinkProgram(shaderProgram[0]);

    glAttachShader(shaderProgram[1], vertexShader);
    glAttachShader(shaderProgram[1], fragmentShader[1]);
    glLinkProgram(shaderProgram[1]);

    glAttachShader(shaderProgram[2], vertexShader);
    glAttachShader(shaderProgram[2], fragmentShader[2]);
    glLinkProgram(shaderProgram[2]);   
    
    glAttachShader(shaderProgram[3], vertexShader);
    glAttachShader(shaderProgram[3], fragmentShader[3]);
    glLinkProgram(shaderProgram[3]);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader[0]);
    glDeleteShader(fragmentShader[1]);
    glDeleteShader(fragmentShader[2]);
    glDeleteShader(fragmentShader[3]);

    //object
    GLuint VAO[4];
    glGenVertexArrays(4, VAO);    
    GLuint VBO[4];
    glGenBuffers(4, VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rVertices), rVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(r1Vertices), r1Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(r2Vertices), r2Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(pcolor->GetR(), pcolor->GetG(), pcolor->GetB(), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GLint modes[2];
        glGetIntegerv(GL_POLYGON_MODE, modes);

        if (modes[0] == GL_LINE)
        {
            modes[0] = GL_LINE;
            modes[1] = GL_FILL;
        }
        else
        {
            modes[0] == GL_FILL;
            modes[1] = GL_LINE;
        }
        DrawTop(VAO, shaderProgram);
        DrawUnder(VAO, shaderProgram, modes);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
