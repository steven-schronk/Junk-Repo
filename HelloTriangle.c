/*

sudo apt-get install libgl1-mesa-dev libgles2-mesa-dev libegl-dev libdrm-dev libgbm-dev

sudo apt-get install libglew-dev

sudo apt-get install freeglut3-dev

gcc -o HelloTriangle HelloTriangle.c -lGL -lGLU -lglut -lGLEW

*/


#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

GLuint program;

const GLchar* vertexShaderSource = "#version 300 es\n"
    "in vec2 position;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\0";

int init() {
    GLuint vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(program);
    return 1;

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Simple Shader");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glewExperimental = GL_TRUE;
    glewInit();

    initGL();
    init();

    glutMainLoop();

    return 0;
}


