#include "rubik.h"
#include <cmath>
#include <stack>
#define LIMIT_ITERATION 5000

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

uint8_t Rubik::moves_need(Cube& cube) {
    FACE face_colors[FACE_COUNT] {};
    for (Face f = 0; f < FACE_COUNT; f++) {
        size_t idx = IDX_FACE(f);
        assert(cache_cubes[idx].size() == 1 && (*cache_cubes[idx].begin())->nFaces == 1);
        face_colors[(*cache_cubes[idx].begin())->colors[f]] = (FACE)f;
    }
    uint8_t moves = 0;
    if (cube.nFaces == 2) {
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube.faces[f]) {
                FACE true_face_pos = face_colors[cube.colors[f]];
                moves += f == true_face_pos ? 0 : opposite_faces[f] == true_face_pos ? 2 : 1;
            }
        }
    }
    else if (cube.nFaces == 3) {
        uint8_t mn = 0xff, mx = 0;
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cube.faces[f]) {
                FACE true_face_pos = face_colors[cube.colors[f]];
                uint8_t cur_need = f == true_face_pos ? 0 : opposite_faces[f] == true_face_pos ? 2 : 1;
                mn = std::min(cur_need, mn);
                mx = std::max(cur_need, mx);
            }
        }
        moves += mn + mx;
    }
    return moves;
}

uint8_t Rubik::heuristic() {
    uint8_t edge_error = 0;
    uint8_t corner_error = 0;
    for (size_t i = 0; i < all_cubes.size(); i++) {
        if (all_cubes[i]->nFaces == 2) edge_error += moves_need(*all_cubes[i]);
        else if (all_cubes[i]->nFaces == 3) corner_error += moves_need(*all_cubes[i]);
    }
    return std::max((uint8_t)ceil(edge_error / 4.0), (uint8_t)ceil(corner_error / 4.0));
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

bool Rubik::solve(uint8_t threshold, uint8_t depth, RUBIK_ROTATE_OP prev, std::list<RUBIK_ROTATE_OP>* ops, uint8_t* new_threshold) {
    printf("Depth: %8u\n", depth);
    if (finish()) {
        return true;
    }
    if (depth >= 10) {
        printf("Cut because depth exceed 10\n");
        return false;
    }
    for (uint8_t op = 0; op < RUBIK_OP_COUNT; op++) {
        if (prev != RUBIK_OP_COUNT && checkRedundantOP(prev, (RUBIK_ROTATE_OP)op)) {
            // printf("Skip op at depth %zu\n", depth);
            continue;
        }
        rotate((RUBIK_ROTATE_OP)op);
        ops->push_back((RUBIK_ROTATE_OP)op);
        // printf("        OP: %s\n", rubik_op_name[op]);
        uint8_t f_value = depth + heuristic();
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
bool Rubik::solve(SolverStackData root_data, uint8_t* new_threshold) {
    std::stack<SolverStackData> stack;
    stack.push(root_data);
    uint8_t depth = 1;
    printf("Depth: %8u\n", depth);
    while (stack.size() != 0 && !finish()) {
        auto& top = stack.top();
        top.cur++;
        if (depth >= 10 || top.cur == RUBIK_OP_COUNT) {
            if (depth >= 10) printf("Depth exceed 10, cut off\n");
            stack.pop();
            if (stack.size() != 0) rotate(reverse_rubik_op[stack.top().cur]);
        }
        else if (top.prev == NO_OPERATION || !checkRedundantOP((RUBIK_ROTATE_OP)top.prev, (RUBIK_ROTATE_OP)top.cur)) {
            rotate((RUBIK_ROTATE_OP)top.cur);
            uint8_t f_value = heuristic() + depth;
            if (f_value <= top.threshold) { // node can be explored, move down new depth
                depth++;
                printf("Depth: %8u\n", depth);
                stack.push(SolverStackData {.prev = top.cur, .cur = NO_OPERATION, .threshold = f_value});
            }
            else { // can't explore this node, reverse rotation, update new_threshold
                rotate(reverse_rubik_op[top.cur]);
                *new_threshold = std::min(*new_threshold, f_value);
            }
        }
    }
    return stack.size() != 0;
}

void Rubik::solve() {
    static size_t count = 0;
    printf("        Start solving %zu...\n", count);
    std::list<RUBIK_ROTATE_OP> ops;
    uint8_t new_threshold = 0xff;
    uint8_t threshold = heuristic();
    size_t iteration_count = 0;
#if 0
    while (!solve(threshold, 1, RUBIK_OP_COUNT, &ops, &new_threshold) && iteration_count < LIMIT_ITERATION) { // no prev operation
        threshold = new_threshold;
        new_threshold = 0xff;
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
#else
    while (iteration_count < LIMIT_ITERATION &&
          !solve(SolverStackData {.prev = NO_OPERATION, .cur = NO_OPERATION, .threshold = threshold}, &new_threshold)) {
        threshold = new_threshold;
        new_threshold = 0xff;
        iteration_count++;
    }
    if (finish()) {
        printf("Solution found at iteration %zu\n", iteration_count);
    }
    else {
        printf("Rubik can't be solved within %u\n", LIMIT_ITERATION);
    }
#endif
    count++;
}

