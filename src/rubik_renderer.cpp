#include "rubik_renderer.h"
#include "Utils.h"

inline static constexpr Vec3 cube_vertexes[] {
    {1.000000f, 1.000000f, -1.000000f},
    {1.000000f, -1.000000f, -1.000000f},
    {1.000000f, 1.000000f, 1.000000f},
    {1.000000f, -1.000000f, 1.000000f},
    {-1.000000f, 1.000000f, -1.000000f},
    {-1.000000f, -1.000000f, -1.000000f},
    {-1.000000f, 1.000000f, 1.000000f},
    {-1.000000f, -1.000000f, 1.000000f},
};

// enum FACE_DIRECTION {
//     DOWN = 0,
//     UP,
//     RIGHT,
//     LEFT,
//     FRONT,
//     BACK,
//     FACE_COUNT,
// };
void init_reference_cube() {
    RubikRenderer::reference_cube.faces[UP] = Quad {
        .v = {cube_vertexes[4],
              cube_vertexes[6],
              cube_vertexes[2],
              cube_vertexes[4],
              cube_vertexes[2],
              cube_vertexes[0],
        },
    };
    RubikRenderer::reference_cube.faces[FRONT] = Quad {
        .v = {cube_vertexes[2],
              cube_vertexes[6],
              cube_vertexes[7],
              cube_vertexes[2],
              cube_vertexes[7],
              cube_vertexes[3]},
    };
    RubikRenderer::reference_cube.faces[RIGHT] = Quad {
        .v = {cube_vertexes[0],
              cube_vertexes[2],
              cube_vertexes[3],
              cube_vertexes[0],
              cube_vertexes[3],
              cube_vertexes[1]},
    };
    RubikRenderer::reference_cube.faces[BACK] = Quad {
        .v = {cube_vertexes[4],
              cube_vertexes[0],
              cube_vertexes[1],
              cube_vertexes[4],
              cube_vertexes[1],
              cube_vertexes[5]},
    };
    RubikRenderer::reference_cube.faces[LEFT] = Quad {
        .v = {cube_vertexes[6],
              cube_vertexes[5],
              cube_vertexes[7],
              cube_vertexes[6],
              cube_vertexes[4],
              cube_vertexes[5]},
    };
    RubikRenderer::reference_cube.faces[DOWN] = Quad {
        .v = {cube_vertexes[1],
              cube_vertexes[3],
              cube_vertexes[7],
              cube_vertexes[1],
              cube_vertexes[7],
              cube_vertexes[5]},
    };
}

inline static CubeVertex gen_cube_at(size_t idx_cube) {
    CubeVertex new_cube {RubikRenderer::reference_cube};
    const float padding = .3f;
    const float size = 2.0f;
    const float base_off = size + padding;
    Vec3 offset {}; 
    if ((idx_cube & IDX_FACE(UP)) != 0)     offset.y += base_off;
    if ((idx_cube & IDX_FACE(DOWN)) != 0)   offset.y -= base_off;
    if ((idx_cube & IDX_FACE(RIGHT)) != 0)  offset.x += base_off;
    if ((idx_cube & IDX_FACE(LEFT)) != 0)   offset.x -= base_off; 
    if ((idx_cube & IDX_FACE(FRONT)) != 0)  offset.z += base_off; 
    if ((idx_cube & IDX_FACE(BACK)) != 0)   offset.z -= base_off; 
    assert(offset.x > 1.0f || offset.x < -1.0f ||
           offset.y > 1.0f || offset.y < -1.0f ||
           offset.z > 1.0f || offset.z < -1.0f);
    for (size_t i = 0; i < FACE_COUNT; i++) {
        for (size_t j = 0; j < Quad::VERT_COUNT; j++) {
            new_cube.faces[i].v[j].x += offset.x;
            new_cube.faces[i].v[j].y += offset.y;
            new_cube.faces[i].v[j].z += offset.z;
        }
    }
    return new_cube;
}

void RubikRenderer::update_color() {
    for (size_t i = 0; i < CUBE_COUNT; i++) {
        size_t cube_idx = cube_indexes[i];
        assert(data->cache_cubes[cube_idx].size() == 1);
        auto& cube_data = **data->cache_cubes[cube_idx].begin();
        assert(cache_index[cube_idx] != -1);
        cube_colors[cache_index[cube_idx]] = cube_data.colors;
    }
}
void RubikRenderer::sync_color_buffer() {
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer); {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_colors), cube_colors);
    } glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RubikRenderer::gen_init_cubes() {
    for (size_t i = 0; i < CUBE_COUNT; i++) {
        size_t idx = cube_indexes[i];
        assert(data->cache_cubes[idx].size() == 1);
        assert(cache_index[idx] != -1);
        cube_vertices[cache_index[idx]] = gen_cube_at(idx);
    }
}

RubikRenderer::RubikRenderer(const Rubik* r): data{r} {
    // this process is similar to one i do in rubik.cpp and the initial indexes are the same too
    // but the indexes here won't change (for example index 0 will always store position of 1 face down cube)
    // and the index 0 of all_cubes array could change to 1 face up cube
    // that's why caching is needed, i hope my stupid comments help a little
    cache_index.fill(-1); // so that we know which index has no data
    for (size_t i = 0; i < CUBE_COUNT; i++) {
        size_t idx = data->all_cubes[i]->get_index();
        cube_indexes[i] = idx;
        cache_index[idx] = i;
    }
    extern size_t SCREEN_WIDTH;
    extern size_t SCREEN_HEIGHT;
    init_reference_cube();
    gen_init_cubes(); 
    update_color();
    program = create_program(shader_files, shader_types, n_shaders);
    glUseProgram(program);

    cam.x = 5.0f;
    cam.y = 5.0f;
    cam.z = 20.0f;
    static GLint resolution_loc = glGetUniformLocation(program, "resolution");
    static GLint proj_loc = glGetUniformLocation(program, "proj_mat");
    static GLint v_loc = glGetUniformLocation(program, "view_mat");
    glUniform2f(resolution_loc, SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniformMatrix4fv(proj_loc,
                       1, GL_FALSE,
                       glm::value_ptr(glm::perspective(1.0472f, SCREEN_WIDTH * 1.0f / SCREEN_HEIGHT, 0.1f, 1000.0f)));
    glUniformMatrix4fv(
            v_loc,
            1,
            GL_FALSE, 
            glm::value_ptr( glm::translate(glm::mat4(1.0f), -cam)));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &color_buffer);
    glBindVertexArray(vao); {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); {
            printf("size all cubes: %zu\n", sizeof(cube_vertices));
            printf("size 1 cube: %zu\n", sizeof(cube_vertices[0]));
            glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        } glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer); {
            printf("size all cubes color: %zu\n", sizeof(cube_colors));
            printf("size 1 cube color: %zu\n", sizeof(cube_colors[0]));
            glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(GLubyte), (GLvoid*)0);
        } glBindBuffer(GL_ARRAY_BUFFER, 0);
    } glBindVertexArray(0);
                    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
void RubikRenderer::render(float time) {
    static GLint time_loc = glGetUniformLocation(program, "time");
    glUniform1f(time_loc, time);
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao); {
        update_color();    
        sync_color_buffer();
        glDrawArrays(GL_TRIANGLES, 0, 36 * CUBE_COUNT);
    } glBindVertexArray(0);
    if (checkOpenGLError()) exit(3);
}
