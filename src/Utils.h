#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <fstream>
#include <sstream>

// seperate an 32 bits integer into 4 8 bits color (r g b a)
#define UNHEX(type, c) (type)((uint8_t)((c) >> 8 * 3) & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c) >> 8 * 2) & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c) >> 8)     & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c))          & (uint8_t)0xff)

// check for error code
inline void check(int code)
{
    if (code != 0) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
}

// check for pointer
template<typename T>
inline T check(T data)
{
    if (!data) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    return data;
}
std::string readAllFile(const char* filepath);

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam);
GLuint create_compiled_shader(const char* source, GLenum type);
GLuint create_program(const char** src, const GLenum* type, size_t n);
