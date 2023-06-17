#include "rubik.h"
#include "rubik_renderer.h"
#include "Utils.h"
#include <iostream>
#include <SDL2/SDL.h>

#define FPS 60
inline size_t SCREEN_WIDTH = 800;
inline size_t SCREEN_HEIGHT = 600;

int main() {
    printf("size: %zu\n", sizeof(CubeVertex));
    check(SDL_Init(SDL_INIT_VIDEO));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
// init stuff 
    SDL_Window* window = check(SDL_CreateWindow("gnout",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));
    int gl_ver_major, gl_ver_minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_ver_major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_ver_minor);
    printf("OpenGL %d.%d\n", gl_ver_major, gl_ver_minor);

    SDL_GLContext context = check(SDL_GL_CreateContext(window));
    SDL_GL_MakeCurrent(window, context);

    if (GLEW_OK != glewInit()) {
        fprintf(stderr, "Can't init glew\n");
        exit(EXIT_FAILURE);
    }

    if (glDebugMessageCallback != NULL) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }

    Rubik rubik;
    printf("RR: %zu\n", sizeof(RubikRenderer));
    RubikRenderer renderer{&rubik};
    SDL_Event event {};
    bool quit = false;
    while (!quit) {
        static bool onShift = false;
        uint32_t start_tick = SDL_GetTicks(); // start tick for fps handle
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                quit = true;
            } break;
//          R_U = 0, R_D, R_R, R_L, R_F, R_B
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                case SDLK_u: rubik.rotate(onShift ? R_ReU : R_U); break;
                case SDLK_d: rubik.rotate(onShift ? R_ReD : R_D); break;
                case SDLK_r: rubik.rotate(onShift ? R_ReR : R_R); break;
                case SDLK_l: rubik.rotate(onShift ? R_ReL : R_L); break;
                case SDLK_f: rubik.rotate(onShift ? R_ReF : R_F); break;
                case SDLK_b: rubik.rotate(onShift ? R_ReB : R_B); break;
                case SDLK_s: rubik.solve(); break;

                case SDLK_LSHIFT: onShift = true; break;
                case SDLK_SPACE: printf("Error: %8u\n", rubik.heuristic()); break;
                } break;
            } break;
            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
                case SDLK_LSHIFT: onShift = false; break;
                } break;
            } break;
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SCREEN_WIDTH = event.window.data1;
                    SCREEN_HEIGHT = event.window.data2;
                    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                }
            } break;
            }
        }
        renderer.render(start_tick / 1000.0f);
        
        SDL_GL_SwapWindow(window);
        uint32_t interval = SDL_GetTicks() - start_tick;
        if (interval < 1000 / FPS) {
            SDL_Delay(1000 / FPS - interval);
        }
    }
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    return 0;
}
