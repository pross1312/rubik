#include "rubik.h"
#include <stdio.h>
#include <stdlib.h>

size_t Cube::get_index() {
    size_t idx = 0;
    for (Face f = 0; f < faces.size(); f++) {
        if (faces[f]) {
            idx |= IDX_FACE(f);
        }
    }
    return idx;
}
void Cube::dump() {
    for (Face f = 0; f < faces.size(); f++) {
        if (faces[f]) {
            print_face((FACE)f);
            printf(" ");
        }
    }
    printf("\n");
}

bool Rubik::finish() {
    for (Face f = 0; f < FACE_COUNT; f++) {
        size_t face_idx = WHOLE_FACE(f);
        COLOR c = (*cache_cubes[face_idx].begin())->colors[f]; // one face cube
        assert((*cache_cubes[face_idx].begin())->nFaces == 1);
        for (auto& cube : cache_cubes[WHOLE_FACE(f)]) {
            if (cube->colors[f] != c) return false;
        }
    }
    return true;
}

// ----- all_cubes construct -------
Rubik::Rubik() {
    for (auto& cube : all_cubes) {
        cube = std::make_shared<Cube>();
    }
    // init center cubes
    for (Face f = 0; f < FACE_COUNT; f++) {
        all_cubes[f]->set(f);
    }
    
    // init tri_face_cubes
    // U U U U D D D D
    // F F B B F F B B
    // L R L R L R L R
    size_t tri_cube_count = 8;
    for (size_t i = 0; i < tri_cube_count; i++) {
        all_cubes[i + FACE_COUNT]->set(i < 4 ? UP : DOWN);
        all_cubes[i + FACE_COUNT]->set((i / 2) % 2 == 0 ? FRONT : BACK);
        all_cubes[i + FACE_COUNT]->set((i % 2) == 0 ? LEFT : RIGHT);
    }
    // init bi_face_cubes D D D D U U U U R R L L
    // R L F B R L F B F B F B
    size_t count = FACE_COUNT + tri_cube_count;
    for (Face f1 = 0; f1 < FACE_COUNT - 1; f1++) {
        FACE oppose = get_oppose((FACE)f1);
        for (Face f2 = f1 + 1; f2 < FACE_COUNT; f2++) {
            if (f2 != oppose) {
                all_cubes[count]->set(f1);
                all_cubes[count]->set(f2);
                count++;
            }
        }
    }
    for (size_t i = 0; i < all_cubes.size(); i++) {
        all_cubes[i]->dump(); 
    }
    // init color and faces
    // caching cubes using value of 'or all faces' (Ex: IDX_FACE(UP) | IDX_FACE(RIGHT), ...)
    for (auto& cube : all_cubes) {
        size_t idx = 0;
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube->faces[f]) {
                idx |= IDX_FACE(f);
                cache_cubes[WHOLE_FACE(f)].push_back(cube);
            }
        }
        printf("FACE: %zu\n", idx);
        assert(cache_cubes[idx].size() == 0);
        cache_cubes[idx].push_back(cube);
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube->hasFace((FACE)f)) {
                cube->colors[f] = (COLOR)f;
            }
        }
    }
    for (Face f = 0; f < FACE_COUNT; f++) {
        assert(cache_cubes[WHOLE_FACE(f)].size() == 9);
    }
    for (size_t i = 0; i < 100; i++) {
        RUBIK_ROTATE_OP op = (RUBIK_ROTATE_OP)(rand() % ROTATE_OP_COUNT);
        this->rotate(op);
    }
}
