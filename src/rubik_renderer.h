#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rubik.h"

inline const char* shader_files[] {
    "shaders/vShader.glsl",
    "shaders/fShader.glsl",
};
inline constexpr GLenum shader_types[] {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER       
};


inline constexpr const size_t n_shaders = sizeof(shader_files) / sizeof(shader_files[0]);
static_assert(n_shaders == sizeof(shader_types) / sizeof(shader_types[0]));

struct Vec3 {
    float x, y, z;
};

struct Quad {
    inline static const size_t VERT_COUNT = 6;
    Vec3 v[VERT_COUNT];
};

// color of every vertex of a cube
struct CubeColor {
    unsigned char colors[FACE_COUNT * Quad::VERT_COUNT];
    CubeColor() { memset(colors, NONE, FACE_COUNT * Quad::VERT_COUNT); }
    CubeColor(const CubeColor& base) {
        memcpy(colors, base.colors, FACE_COUNT * Quad::VERT_COUNT);
    }
    CubeColor& operator=(const CubeColor& base) {
        memcpy(colors, base.colors, FACE_COUNT * Quad::VERT_COUNT);
        return *this;
    }
    CubeColor& operator=(std::array<COLOR, FACE_COUNT>& c) {
        for (Face f = 0; f < FACE_COUNT; f++) {
            memset(colors + FACE_COUNT * f, c[f], Quad::VERT_COUNT);
        }
        return *this;
    }
};

// position of every vertex of a cube
struct CubeVertex {
    Quad faces[FACE_COUNT];
    CubeVertex() = default;
    CubeVertex(const CubeVertex& base) {
        memcpy(faces, base.faces, sizeof(faces));
    }
    CubeVertex& operator=(const CubeVertex& base) {
        memcpy(faces, base.faces, sizeof(faces));
        return *this;
    }
};


class RubikRenderer {
public:
    RubikRenderer(const Rubik* r);

    void render(float time);
    void update_color();
    inline static CubeVertex reference_cube;
    
private:
    void sync_color_buffer();
    void gen_init_cubes();

    GLuint program;
    GLuint vao;
    GLuint vertex_buffer;
    GLuint color_buffer;
    glm::vec3 cam;
    CubeVertex cube_vertices[CUBE_COUNT];
    CubeColor  cube_colors[CUBE_COUNT];
    inline static std::array<int, 1<<FACE_COUNT> cache_index;  
    inline static std::array<int, CUBE_COUNT> cube_indexes;
    // data is store in a special way to access any cubes in constant time, but CubeVertex and Color aren't
    // so we need a special cache to cache the index to access data quickly ...
    const Rubik* data;
};
