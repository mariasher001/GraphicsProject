#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <string>
#include <fstream>
using namespace std;

int windowWidth = 700;
int windowHeight = 700;
char windowTitle[] = "Assignment2";
int drag = -1;
GLFWwindow* window = nullptr;
GLuint VBO[2];
GLuint VAO[1];
GLuint shaderProgram;

float vertices[] = {
    -0.5, -0.5, 0.0,        //P1
    -0.5,  0.5, 0.0,        //P2
     0.5,  0.5, 0.0,        //P3
     0.5, -0.5, 0.0,        //P4
    -0.5, -0.5, 0.0,        //P5 
    -0.5,  0.5, 0.0,        //P6 
     0.5,  0.5, 0.0,        //P7 
    -0.5, -0.5, 0.0,        //P8 
    -0.5,  0.5, 0.0,        //P9 
    -0.5, -0.5, 0.0         //BZ 
};


float vertexColors[] =
{
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0
    
};

GLuint createShader();
string readShaderSource(string filepath);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


void controlPoints() {
    glBindVertexArray(VAO[0]);
    glPointSize(10.0);
    glColor3f(1.0, 0.0, 0.0);
    glDrawArrays(GL_POINTS, 0, 4);
    glBindVertexArray(0);
}

void lines() {
    glBindVertexArray(VAO[0]);
    glLineWidth(2.0);
    glColor3f(0.0, 0.0, 1.0);
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_LINES, 1, 2);
    glDrawArrays(GL_LINES, 2, 2);
    glBindVertexArray(0);
}


void pointChanges(float t) {
    vertices[12] = vertices[0] * (1 - t) + vertices[3] * t;
    vertices[13] = vertices[1] * (1 - t) + vertices[4] * t;
    vertices[15] = vertices[3] * (1 - t) + vertices[6] * t;
    vertices[16] = vertices[4] * (1 - t) + vertices[7] * t;
    vertices[18] = vertices[6] * (1 - t) + vertices[9] * t;
    vertices[19] = vertices[7] * (1 - t) + vertices[10] * t;
    vertices[21] = vertices[12] * (1 - t) + vertices[15] * t;
    vertices[22] = vertices[13] * (1 - t) + vertices[16] * t;
    vertices[24] = vertices[15] * (1 - t) + vertices[18] * t;
    vertices[25] = vertices[16] * (1 - t) + vertices[19] * t;
    vertices[27] = vertices[21] * (1 - t) + vertices[24] * t;
    vertices[28] = vertices[22] * (1 - t) + vertices[25] * t;
}

void plotCurve() {
    glBegin(GL_POINTS);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(vertices[27], vertices[28], vertices[29]);
    glEnd();
}

void curvePointschanges() {
    for (float t = 0.0; t <= 1.0; t += 0.001) {
        pointChanges(t);
        plotCurve();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    curvePointschanges();
    lines();
    controlPoints();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    shaderProgram = createShader();
    glGenBuffers(2, VBO);
    glGenVertexArrays(1, VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(shaderProgram);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glViewport(0, 0, 700, 700);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
    glfwSwapInterval(1);
    glfwSetWindowSizeLimits(window, 700, 700, 700, 700);
    glfwSetWindowAspectRatio(window, 1, 1);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);


    init();
    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    if (drag >= 0) {
        float xNorm = xPos / (windowWidth / 2) - 1.0;
        float yNorm = -1 * (yPos / (windowHeight / 2) - 1.0);

        vertices[drag * 3] = xNorm;
        vertices[drag * 3 + 1] = yNorm;

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

int getActivePoints(double x, double y) {
    float xNorm = x / (windowWidth / 2) - 1.0;
    float yNorm = -1 * (y / (windowHeight / 2) - 1.0);

    if (xNorm > vertices[0] - 0.02 && xNorm < vertices[0] + 0.02 && yNorm > vertices[1] - 0.02 && yNorm < vertices[1] + 0.02) {
        return 0;
    }
    else if (xNorm > vertices[3] - 0.02 && xNorm < vertices[3] + 0.02 && yNorm > vertices[4] - 0.02 && yNorm < vertices[4] + 0.02) {
        return 1;
    }
    else if (xNorm > vertices[6] - 0.02 && xNorm < vertices[6] + 0.02 && yNorm > vertices[7] - 0.02 && yNorm < vertices[7] + 0.02) {
        return 2;
    }
    else if (xNorm > vertices[9] - 0.02 && xNorm < vertices[9] + 0.02 && yNorm > vertices[10] - 0.02 && yNorm < vertices[10] + 0.02) {
        return 3;
    }
    else {
        return -1;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double	x, y;
        glfwGetCursorPos(window, &x, &y);
        drag = getActivePoints(x, y);
        cout << endl << "Drag: " << drag << endl;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        drag = -1;
    }
}


GLuint createShader() {

    string vertexShaderStr = readShaderSource("VertexShader.glsl");
    const char* vertexShaderSource = vertexShaderStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    string fragmentShaderstr = readShaderSource("FragmentShader.glsl");
    const char* fragmentShaderSource = fragmentShaderstr.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

string readShaderSource(string filepath) {
    ifstream	fileStream(filepath, ios::in);
    string		content;
    string		line;

    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}