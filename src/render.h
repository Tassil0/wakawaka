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

#endif   // WAKAWAKA_RENDER_H
