//
// Created by filip on 18.11.22.
//

#include <SDL2/SDL.h>

#ifndef WAKAWAKA_TYPES_H
#define WAKAWAKA_TYPES_H

typedef struct {
    void (*logic)(void);

    void (*render)(void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct {
    int w;
    int h;
    float x;
    float y;
    float dx;
    float dy;
    SDL_Texture *texture;
} PlayerEntity;

typedef struct {
    PlayerEntity *player;
} Stage;

#endif // WAKAWAKA_TYPES_H
