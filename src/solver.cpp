#include "rubik.h" #include <cmath>

#define LIMIT_ITERATION 10000

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

size_t Rubik::moves_need(Cube& cube) {
    FACE face_colors[FACE_COUNT] {};
    for (Face f = 0; f < FACE_COUNT; f++) {
        size_t idx = IDX_FACE(f);
        assert(cache_cubes[idx].size() == 1 && (*cache_cubes[idx].begin())->nFaces == 1);
        face_colors[(*cache_cubes[idx].begin())->colors[f]] = (FACE)f;
    }
    size_t moves = 0;
    if (cube.nFaces == 2) {
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube.faces[f]) {
                FACE true_face_pos = face_colors[cube.colors[f]];
                moves += f == true_face_pos ? 0 : opposite_faces[f] == true_face_pos ? 2 : 1;
            }
        }
    }
    else if (cube.nFaces == 3) {
        size_t mn = 100, mx = 0;
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube.faces[f]) {
                FACE true_face_pos = face_colors[cube.colors[f]];
                size_t cur_need = f == true_face_pos ? 0 : opposite_faces[f] == true_face_pos ? 2 : 1;
                mn = std::min(cur_need, mn);
                mx = std::max(cur_need, mx);
            }
        }
        moves += mn + mx;
    }
    return moves;
}

size_t Rubik::heuristic() {
    size_t edge_error = 0;
    size_t corner_error = 0;
    for (size_t i = 0; i < all_cubes.size(); i++) {
        if (all_cubes[i]->nFaces == 2) edge_error += moves_need(*all_cubes[i]);
        else if (all_cubes[i]->nFaces == 3) corner_error += moves_need(*all_cubes[i]);
    }
    return std::max((size_t)ceil(edge_error / 4.0), (size_t)ceil(corner_error / 4.0));
}
bool checkRedundantOP(RUBIK_ROTATE_OP prev, RUBIK_ROTATE_OP next) {
    switch (prev) {
    case R_U: case R_ReU: case R_2U: return next == R_ReU || next == R_2U || next == R_U;
    case R_D: case R_ReD: case R_2D: return next == R_ReD || next == R_2D || next == R_D;
    case R_R: case R_ReR: case R_2R: return next == R_ReR || next == R_2R || next == R_R;
    case R_L: case R_ReL: case R_2L: return next == R_ReL || next == R_2L || next == R_L;
    case R_F: case R_ReF: case R_2F: return next == R_ReF || next == R_2F || next == R_F;
    case R_B: case R_ReB: case R_2B: return next == R_ReB || next == R_2B || next == R_B;
    default:
        assert(false && "Undefined rotate operation");
    }
    return false;
}

bool Rubik::solve(size_t threshold, size_t depth, RUBIK_ROTATE_OP prev, std::list<RUBIK_ROTATE_OP>* ops, size_t* new_threshold) {
    printf("Depth: %zu\n", depth);
    if (finish()) {
        return true;
    }
    if (depth >= 10) {
        printf("Cut because depth exceed 10\n");
        return false;
    }
    for (size_t op = 0; op < RUBIK_OP_COUNT; op++) {
        if (prev != RUBIK_OP_COUNT && checkRedundantOP(prev, (RUBIK_ROTATE_OP)op)) {
            // printf("Skip op at depth %zu\n", depth);
            continue;
        }
        rotate((RUBIK_ROTATE_OP)op);
        ops->push_back((RUBIK_ROTATE_OP)op);
        // printf("        OP: %s\n", rubik_op_name[op]);
        size_t f_value = depth + heuristic();
        // printf("F: %zu\n", f_value);
        if (f_value <= threshold) {
            if (solve(f_value, depth + 1, (RUBIK_ROTATE_OP)op, ops, new_threshold)) return true;
        }
        ops->pop_back();
        // printf("Reverse OP: %s\n", rubik_op_name[reverse_rubik_op[op]]);
        rotate(reverse_rubik_op[op]);
        *new_threshold = std::min(*new_threshold, f_value);
    }
    return false;
}

void Rubik::solve() {
    static size_t count = 0;
    printf("        Start solving %zu...\n", count);
    std::list<RUBIK_ROTATE_OP> ops;
    size_t new_threshold = INT32_MAX;
    size_t threshold = heuristic();
    size_t iteration_count = 0;
    while (!solve(threshold, 1, RUBIK_OP_COUNT, &ops, &new_threshold) && iteration_count < LIMIT_ITERATION) { // no prev operation
        threshold = new_threshold;
        new_threshold = INT32_MAX;
        iteration_count++;
    }
    if (ops.size() != 0) {
        printf("Solution found at iteration %zu\n", iteration_count);
        for (auto& op : ops) {
            printf("%s, ", rubik_op_name[op]);
        }
        printf("\n");
    }
    else {
        printf("Can't find solution with current limit %u\n", LIMIT_ITERATION);
    }
    count++;
}

