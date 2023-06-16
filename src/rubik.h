#pragma once
#include <assert.h>
#include <stdio.h>
#include <array>
#include <list>
#include <memory>

template<typename T>
using ptr = std::shared_ptr<T>;

#define CUBE_COUNT 26

#define IDX_FACE(f) (1 << (f))
// this can be defined however you want as long as its doesn't conflict with anything
#define WHOLE_FACE(f) (uint8_t)(((~UINT8_C(f)) & UINT8_C(0b111111)))
using Face = size_t;

enum FACE {
    DOWN = 0,
    UP,
    RIGHT,
    LEFT,
    FRONT,
    BACK,
    FACE_COUNT,
};
inline static const FACE opposite_faces[FACE_COUNT] {
    [DOWN] = UP,
    [UP] = DOWN,
    [RIGHT] = LEFT,
    [LEFT] = RIGHT,
    [FRONT] = BACK,
    [BACK] = FRONT
};





static_assert(FACE_COUNT == 6 && "Unknown face direction");

// U - Yellow
// D - White
// R - Red
// L - Orange
// F - Blue
// B - Green
// order is important as each face is assigned a specific color initially
enum COLOR { 
    WHITE = 0,
    YELLOW,
    RED,
    ORANGE,
    BLUE,
    GREEN,
    NONE,
};
static_assert(NONE == 6 && "Should only have 6 colors and 1 none additional");

inline static void print_face(FACE f) {
    switch (f) {
    case DOWN: printf("DOWN"); return;
    case UP: printf("UP"); return;
    case RIGHT: printf("RIGHT"); return;
    case LEFT: printf("LEFT"); return;
    case FRONT: printf("FRONT"); return;
    case BACK: printf("BACK"); return;
    default: assert(false && "unreachable, undefined face direction");
    }
}

// look along with the axis to get the right direction
// x point right
// y point up
// z point forward (into the depth of the screen)
enum CUBE_ROTATE_DIRECTION {
    X_COUNTER_CLOCK_WISE = 0,
    Y_COUNTER_CLOCK_WISE,
    Z_COUNTER_CLOCK_WISE,
    X_CLOCK_WISE,
    Y_CLOCK_WISE,
    Z_CLOCK_WISE,
    COUNT_CUBE_ROTATE_DIRECTION
};

enum RUBIK_ROTATE_OP {
    R_U = 0 , R_D   , R_R   , R_L   , R_F   , R_B,
    R_ReU   , R_ReD , R_ReR , R_ReL , R_ReF , R_ReB,
    R_2U    , R_2D  , R_2R  , R_2L  , R_2F  , R_2B,
    RUBIK_OP_COUNT,
};
inline static const char* const rubik_op_name[RUBIK_OP_COUNT] {
    "R_D"   , "R_U"   , "R_L"   , "R_R"   , "R_B"   , "R_F",
    "R_ReD" , "R_ReU" , "R_ReL" , "R_ReR" , "R_ReB" , "R_ReF",
    "R_2D"  , "R_2U"  , "R_2L"  , "R_2R"  , "R_2B"  , "R_2F",
};
inline static const RUBIK_ROTATE_OP reverse_rubik_op[RUBIK_OP_COUNT] {
    R_ReU   , R_ReD , R_ReR , R_ReL , R_ReF , R_ReB,
    R_U     , R_D   , R_R   , R_L   , R_F   , R_B,
    R_2U    , R_2D  , R_2R  , R_2L  , R_2F  , R_2B,
};



class Cube { // define a small cube that make up a rubik cube
public:
    Cube(): nFaces{0} { faces.fill(false); colors.fill(NONE); }
    size_t get_index();
    void dump();
    void set(FACE f) {assert(!faces[f]); faces[f] = true; nFaces++;}
    void set(Face f) {assert(!faces[f]); faces[f] = true; nFaces++;}
    bool hasFace(FACE f) { return faces[f]; }
    size_t nFaces;
    std::array<bool, FACE_COUNT> faces; // each cube has 6 faces, but only atmost 3 are used, used faces will have true value
    std::array<COLOR, FACE_COUNT> colors;
};

class Rubik {
public:
    Rubik();
    ~Rubik() = default;
    friend class RubikRenderer;     
    void rotate(RUBIK_ROTATE_OP dir);
    size_t heuristic();
    bool finish();
    void solve();
private:
    bool solve(size_t threshold, size_t depth, RUBIK_ROTATE_OP prev, std::list<RUBIK_ROTATE_OP>* ops, size_t* new_threshold);
    void circular_swap_color(size_t start_cube_idx, CUBE_ROTATE_DIRECTION rot_dir);
    size_t moves_need(Cube& c);

    std::array<ptr<Cube>, CUBE_COUNT> all_cubes; // do not count the center no faces or color cube;

    // to look up every cube in constant time, i will index them by 'or' of all faces that it contains together
    // "ex: ((1 << UP) | (1 << RIGHT) | (1 << BACK))"
    // there will be at most 2^FACE_COUNT entries, a lot more than 9*3-1 cubes
    // so each entry should be a list of pointer to cubes so that we can store things like faces, edges neighbor cubes...
    std::array<std::list<ptr<Cube>>, (1 << FACE_COUNT)> cache_cubes;
};
