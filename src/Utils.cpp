#include "Utils.h"
#include <cmath>
#include <assert.h>

inline std::string readAllFile(const char* filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open()) {
        fprintf(stderr, "Can't open file %s\n", filepath);
        return NULL;
    }
    std::stringstream ss;
    std::string line;
    while (!fin.eof()) {
        std::getline(fin, line);
        if (!line.empty())
            ss << line << "\n";
    }
    fin.close();
    return ss.str();
}

inline void printShaderLog(GLuint shader)
{
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        printf("Shader Info Log: %s\n", log);
        free(log);
    }
}

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        printf("Program Info Log: %s\n", log);
        free(log);
    }
}

bool checkOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        printf("error code: %i, msg:%s\n", glErr, gluErrorString(glErr));
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}
GLuint create_compiled_shader(const char* source, GLenum type)
{
    GLint compile_status;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkOpenGLError();
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != 1) {
        printf("Shader compilation failed\n");
        printShaderLog(shader);
    }
    return shader;
};


GLuint create_program(const char** src, const GLenum* type, size_t n) {
    GLuint vfProgram = glCreateProgram();
    for (size_t i = 0; i < n; i++) {
        auto source_code = readAllFile(src[i]);
        GLuint shader = create_compiled_shader(source_code.c_str(), type[i]);
        glAttachShader(vfProgram, shader);
    }
    glLinkProgram(vfProgram);
    checkOpenGLError();
    GLint linked;
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        printf("Link fail\n");
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

