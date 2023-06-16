#include "rubik.h"
// implement rotation for the rubik //

// enum CUBE_ROTATE_DIRECTION {
//     X_COUNTER_CLOCK_WISE = 0,
//     Y_COUNTER_CLOCK_WISE,
//     Z_COUNTER_CLOCK_WISE,
//     X_CLOCK_WISE,
//     Y_CLOCK_WISE,
//     Z_CLOCK_WISE,
//     COUNT_CUBE_ROTATE_DIRECTION
// };
// Rotate don't change position of any cube in the all_cubes array
// only color of their faces are changed
inline static const FACE new_direction[COUNT_CUBE_ROTATE_DIRECTION][FACE_COUNT] {
//  [INIT_DIRECTION]         {DOWN , UP   , RIGHT, LEFT , FRONT, BACK },
    [X_COUNTER_CLOCK_WISE] = {FRONT, BACK , RIGHT, LEFT , UP   , DOWN }, 
    [Y_COUNTER_CLOCK_WISE] = {DOWN , UP   , FRONT, BACK , LEFT , RIGHT},
    [Z_COUNTER_CLOCK_WISE] = {RIGHT, LEFT , UP   , DOWN , FRONT, BACK },
    [X_CLOCK_WISE]         = {BACK , FRONT, RIGHT, LEFT , DOWN , UP   },
    [Y_CLOCK_WISE]         = {DOWN , UP   , BACK , FRONT, RIGHT, LEFT },
    [Z_CLOCK_WISE]         = {LEFT , RIGHT, DOWN , UP   , FRONT, BACK }
};

// enum RUBIK_ROTATE_OP {
//     R_U = 0, R_D, R_R, R_L, R_F, R_B
// };
void Rubik::rotate(RUBIK_ROTATE_OP op) {
    switch (op) {
    case R_U:
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT), Y_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_COUNTER_CLOCK_WISE);
        break;
    case R_ReU:
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT), Y_CLOCK_WISE);
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_CLOCK_WISE);
        break;
    case R_2U:
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT), Y_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT), Y_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(UP) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_COUNTER_CLOCK_WISE);
        break;
    case R_D:
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT), Y_CLOCK_WISE);
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_CLOCK_WISE);
        break;
    case R_ReD:
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT), Y_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_COUNTER_CLOCK_WISE);
        break;
    case R_2D:
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT), Y_CLOCK_WISE);
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_CLOCK_WISE);
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT), Y_CLOCK_WISE);
        circular_swap_color(IDX_FACE(DOWN) | IDX_FACE(FRONT) | IDX_FACE(LEFT), Y_CLOCK_WISE);
        break;
    case R_R:
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT), X_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_COUNTER_CLOCK_WISE);
        break;
    case R_ReR:
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT), X_CLOCK_WISE);
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_CLOCK_WISE);
        break;
    case R_2R:
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT), X_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT), X_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(RIGHT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_COUNTER_CLOCK_WISE);
        break;
    case R_L:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), X_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_CLOCK_WISE);
        break;
    case R_ReL:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), X_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_COUNTER_CLOCK_WISE);
        break;
    case R_2L:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), X_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), X_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), X_CLOCK_WISE);
        break;
    case R_F:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), Z_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), Z_CLOCK_WISE);
        break;
    case R_ReF:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), Z_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), Z_COUNTER_CLOCK_WISE);
        break;
    case R_2F:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), Z_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), Z_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT), Z_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(FRONT) | IDX_FACE(UP), Z_CLOCK_WISE);
        break;
    case R_B:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK), Z_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK) | IDX_FACE(UP), Z_COUNTER_CLOCK_WISE);
        break;
    case R_ReB:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK), Z_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK) | IDX_FACE(UP), Z_CLOCK_WISE);
        break;
    case R_2B:
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK), Z_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK) | IDX_FACE(UP), Z_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK), Z_COUNTER_CLOCK_WISE);
        circular_swap_color(IDX_FACE(LEFT) | IDX_FACE(BACK) | IDX_FACE(UP), Z_COUNTER_CLOCK_WISE);
        break;
    default:
        assert(false && "Undefined rotate operation");
    }
}


void Rubik::circular_swap_color(size_t start_cube_idx, CUBE_ROTATE_DIRECTION rot_dir) {
    size_t new_cube_idx = 0;
    assert(cache_cubes[start_cube_idx].size() == 1);
    ptr<Cube> cur_cube = *cache_cubes[start_cube_idx].begin();
    std::array<COLOR, FACE_COUNT> new_colors;
    std::array<COLOR, FACE_COUNT> old_colors = cur_cube->colors;
    for (size_t count = 0; count < 4; count++) {
        new_colors.fill(NONE);
        new_cube_idx = 0;
        for (Face f = 0; f < FACE_COUNT; f++) {
            if (cur_cube->faces[f]) {
                new_cube_idx |= IDX_FACE(new_direction[rot_dir][f]);
                new_colors[new_direction[rot_dir][f]] = old_colors[f];
            }
        }
        assert(cache_cubes[new_cube_idx].size() == 1);
        cur_cube = *cache_cubes[new_cube_idx].begin();
        old_colors = cur_cube->colors;
        cur_cube->colors = new_colors;
    }
}
