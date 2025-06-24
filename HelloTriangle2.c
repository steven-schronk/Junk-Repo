/*

sudo apt-get install libgl1-mesa-dev libgles2-mesa-dev libegl-dev libdrm-dev libgbm-dev

sudo apt-get install libglew-dev

sudo apt-get install freeglut3-dev

gcc -o HelloTriangle HelloTriangle.c -lGL -lglut -lGLEW

*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdbool.h>

static struct glData {
    GLuint program;
    GLuint vbo;
} glData;

const char vert_shader_source[] = "#version 300 es                         \n"
                                  "precision mediump float;                \n"
                                  "layout (location = 0) in vec3 Position; \n"
                                  "void main()                             \n"
                                  "{                                       \n"
                                  "   gl_Position = vec4(Position, 1.0);   \n"
                                  "}                                       \n";

const char frag_shader_source[] = "#version 300 es                             \n"
                                  "precision mediump float;                    \n"
                                  "out vec4 fragColor;                         \n"
                                  "void main()                                 \n"
                                  "{                                           \n"
                                  "  fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); \n"
                                  "}                                           \n";

#define POSITION 0

bool initWindow(int* argc, char** argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutCreateWindow("Triangle");
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return false;
    }
    return true;
}

static GLuint buildShader(const char* shader_source, GLenum type)
{
    GLuint shader;
    GLint status;

    shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        int length;
        char* log;

        fprintf(stderr, "failed to compile shader\n");
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            log = calloc(length, sizeof(char));
            glGetShaderInfoLog(shader, length, &length, log);
            fprintf(stderr, "%s\n", log);
            free(log);
        }
        return 0;
    }

    return shader;
}

static GLuint createAndLinkProgram(GLuint v_shader, GLuint f_shader)
{
    GLuint program;
    GLint linked;

    program = glCreateProgram();
    if (program == 0) {
        fprintf(stderr, "failed to create program\n");
        return 0;
    }

    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked != GL_TRUE) {
        int length;
        char* log;

        fprintf(stderr, "failed to link program\n");
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            log = calloc(length, sizeof(char));
            glGetProgramInfoLog(program, length, &length, log);
            fprintf(stderr, "%s\n", log);
            free(log);
        }
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

static bool initProgram()
{
    GLuint v_shader, f_shader;

    v_shader = buildShader(vert_shader_source, GL_VERTEX_SHADER);
    if (v_shader == 0) {
        fprintf(stderr, "failed to build vertex shader\n");
        return false;
    }

    f_shader = buildShader(frag_shader_source, GL_FRAGMENT_SHADER);
    if (f_shader == 0) {
        fprintf(stderr, "failed to build fragment shader\n");
        glDeleteShader(v_shader);
        return false;
    }

    glReleaseShaderCompiler(); // should release resources allocated for the compiler

    glData.program = createAndLinkProgram(v_shader, f_shader);
    if (glData.program == 0) {
        fprintf(stderr, "failed to create and link program\n");
        glDeleteShader(v_shader);
        glDeleteShader(f_shader);
        return false;
    }

    glUseProgram(glData.program);

    // this won't actually delete the shaders until the program is closed but it's a good practice
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    return true;
}

bool setupOpenGL()
{
    if (!initProgram()) {
        fprintf(stderr, "failed to initialize program\n");
        return false;
    }

    GLfloat vVertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };

    glClearColor(0, 0, 0, 1);
    glGenBuffers(1, &glData.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, glData.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    return true;
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
}

void drawTriangle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, glData.vbo);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(POSITION);
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    printf("initialize window\n");
    if (!initWindow(&argc, argv)) {
        fprintf(stderr, "failed to initialize window\n");
        return EXIT_FAILURE;
    }

    printf("setup opengl\n");
    if (!setupOpenGL()) {
        fprintf(stderr, "failed to setup opengl\n");
        return EXIT_FAILURE;
    }

    glutDisplayFunc(drawTriangle);
    glutReshapeFunc(reshape);
    glutMainLoop();

    glDeleteProgram(glData.program);
    return EXIT_SUCCESS;
}
