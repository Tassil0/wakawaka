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

// UNDF = UNDEFINED LMAO
enum directions { LEFT, DOWN, UP, RIGHT, UNDF };

typedef struct {
    int w;
    int h;
    float x;
    float y;
    float dx;
    float dy;
    enum directions currMove;
    enum directions nextMove;
    SDL_Point center;
    SDL_Point gridPos;
    SDL_Rect rect;
    SDL_Texture *texture;
} PlayerEntity;

typedef struct {
    int width;
    int height;
    int *data;
    int rectCount;
    SDL_Texture *texture;
    SDL_Rect *rects;
} Map;

typedef struct {
    PlayerEntity *player;
    Map *map;
} Stage;

#endif   // WAKAWAKA_TYPES_H
