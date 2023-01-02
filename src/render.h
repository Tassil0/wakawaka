//
// Created by filip on 18.11.22.
//

#ifndef WAKAWAKA_RENDER_H
#define WAKAWAKA_RENDER_H

#include "common.h"

void presentScene(void);
void prepareScene(void);
SDL_Texture *loadTexture(const char *path);
void blit(SDL_Texture *texture, int x, int y);
void renderClip(SDL_Texture *texture, SDL_Point *clip, int x, int y);
void renderDiagonals(SDL_Rect *rect);
void setColor(u8 r, u8 g, u8 b, u8 a);
void renderRectDiagonals(SDL_Rect *rect);
void renderGridRect(SDL_Point gridPos);

#endif   // WAKAWAKA_RENDER_H
