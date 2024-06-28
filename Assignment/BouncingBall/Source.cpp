#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<math.h>

GLFWwindow* window = nullptr;

float xPos = 0.0;
float yPos = 0.0;
float radius = 0.2;
float cR = 0.0;
float cG = 1.0;
float cB = 0.0;
const int edge = 50;
const float theeta = 2.0 * (22.0/7.0) / edge;

float bounceX = -0.01;
float bounceY = 0.01;

float lineY1 = 0.0;
float lineY2 = 0.0;
float lineX1 = -0.25;
float lineX2 = 0.25;

void changeCircleColor(int color) {
    if (color == 1) {
        cR = 1.0;
        cG = 0.0;
    }
    else {
        cR = 0.0;
        cG = 1.0;
    }
}

void colorChange() {
    bool flag = false;
    for (float i = lineX1; i <= lineX2;i = i + 0.01) {
        float j = lineY1;
        float distance = (float) sqrt(pow(i - xPos, 2) + pow(j - yPos, 2));
        if (distance <= radius + 0.01 && distance >= radius - 0.01) {
            changeCircleColor(1);
            flag = true;
            break;
        }
    }
    if (!flag) {
        changeCircleColor(0);
    }
}

void processInputforLineMovement(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float stepSize = 0.05;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS && lineY1 <= 0.95) {
        lineY1 += stepSize;
        lineY2 += stepSize;
    } 
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && lineY2 >= -0.95) {
        lineY1 -= stepSize;
        lineY2 -= stepSize;
    }
}

void drawLine() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(lineX1, lineY1, 0.0);
    glVertex3f(lineX2, lineY2, 0.0);
    glEnd();
}

void bounce() {
    if (xPos >= (1.0 - radius) || xPos <= -(1.0 - radius))
        bounceX = -bounceX;
    xPos += bounceX;

    if (yPos >= (1.0 - radius) || yPos <= (radius - 1.0))
        bounceY = -bounceY;
    yPos += bounceY;
}

void drawCircle() {
    float xPos2 = xPos;
    float yPos2 = yPos - radius;

    for (int i = 0; i <= edge; i++) {
        float xPos3 = xPos + radius * sin(theeta*i);
        float yPos3 = yPos - radius * cos(theeta*i);

        glColor4f(cR, cG, cB, 1.0);
        glBegin(GL_TRIANGLES);
        glVertex3f(xPos, yPos, 0.0);
        glVertex3f(xPos2, yPos2, 0.0);
        glVertex3f(xPos3, yPos3, 0.0);
        glEnd();


        xPos2 = xPos3;
        yPos2 = yPos3;
    }
}


void display() {
     
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle();
    drawLine();
    colorChange();
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(700, 700, "Scene1", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glViewport(0, 0, 700, 700);
    
    if (glewInit() != GLEW_OK)
        return 1;

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, processInputforLineMovement);
    glClearColor(1.0, 1.0, 0.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
        bounce();             
    }

   

    glfwDestroyWindow(window );
    glfwTerminate();
    return 0;
}