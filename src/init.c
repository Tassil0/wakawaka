//
// Created by filip on 18.11.22.
//

#include "init.h"
#include "common.h"
#include <SDL_image.h>

extern App app;

void initSDL(void) {
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("WakaWaka", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, windowFlags);

    if (!app.window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    app.gameStarted = false;
}

void cleanup(void) {
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
