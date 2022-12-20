//
// Created by filip on 18.11.22.
//

#include "render.h"
#include "common.h"
#include <SDL_image.h>

extern App app;

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) { SDL_RenderPresent(app.renderer); }

SDL_Texture *loadTexture(const char *path) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                   "Loading texture: %s", path);
    SDL_Texture *texture = IMG_LoadTexture(app.renderer, path);
    if (texture == NULL) {
        SDL_Log("Unable to load texture: %s", SDL_GetError());
        exit(1);
    }
    return texture;
}

void blit(SDL_Texture *texture, int x, int y) {
    SDL_Rect dest = {.x = x, .y = y};
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}
