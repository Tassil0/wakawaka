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

void renderClip(SDL_Texture *texture, SDL_Point *clip, int x, int y) {
    SDL_Rect dest = {.x = x, .y = y, .w = PLAYER_SIZE, .h = PLAYER_SIZE};
    SDL_Rect clipRect = {
        .x = clip->x, .y = clip->y, .w = PLAYER_SIZE, .h = PLAYER_SIZE};
    SDL_RenderCopy(app.renderer, texture, &clipRect, &dest);
}

void renderDiagonals(SDL_Rect *rect) {
    SDL_RenderDrawLine(app.renderer, rect->x, rect->y, rect->x + rect->w,
                       rect->y + rect->h);
    SDL_RenderDrawLine(app.renderer, rect->x, rect->y + rect->h,
                       rect->x + rect->w, rect->y);
}

void renderRectDiagonals(SDL_Rect *rect) {
    SDL_RenderDrawRect(app.renderer, rect);
    renderDiagonals(rect);
}

void setColor(u8 r, u8 g, u8 b, u8 a) {
    SDL_SetRenderDrawColor(app.renderer, r, g, b, a);
}

void renderGridRect(SDL_Point gridPos) {
    SDL_Rect target = (SDL_Rect){.x = gridPos.x * TILE_SIZE - 1,
                                 .y = OFFSET_TOP + gridPos.y * TILE_SIZE - 1,
                                 .w = TILE_SIZE,
                                 .h = TILE_SIZE};
    renderRectDiagonals(&target);
}

void renderGamePoint(int x, int y) {
    setColor(POINT_COLOR, 255);
    SDL_Rect point = (SDL_Rect){.x = x * TILE_SIZE + TILE_SIZE / 2,
                                .y = OFFSET_TOP + y * TILE_SIZE + TILE_SIZE / 2,
                                .w = 5,
                                .h = 5};
    SDL_RenderFillRect(app.renderer, &point);
}

void renderPowerPoint(SDL_Texture *texture, int x, int y) {
    SDL_Rect dest = (SDL_Rect){.x = x * TILE_SIZE + 5,
                               .y = OFFSET_TOP + y * TILE_SIZE + 5,
                               .w = 20,
                               .h = 20};
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}