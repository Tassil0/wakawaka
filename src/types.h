//
// Created by filip on 18.11.22.
//

#include <SDL2/SDL.h>

#ifndef WAKAWAKA_TYPES_H
#define WAKAWAKA_TYPES_H

#include "config.h"
#include <inttypes.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

typedef struct {
    void (*logic)(void);

    void (*render)(void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
    bool gameStarted;
} App;

// UNDF = UNDEFINED LMAO
enum Directions { LEFT, DOWN, UP, RIGHT, UNDF };

typedef struct {
    SDL_Point clip;
    SDL_Point leftClips[4];
    SDL_Point downClips[4];
    SDL_Point upClips[4];
    SDL_Point rightClips[4];
} Animation;

typedef struct {
    int w;
    int h;
    float x;
    float y;
    float dx;
    float dy;
    enum Directions currMove;
    enum Directions nextMove;
    SDL_Point center;
    SDL_Point gridPos;
    SDL_Rect rect;
    // when clyde is in this rectangle he will switch to scatter mode
    SDL_Rect clydeRect;
    u8 lives;
    SDL_Texture *texture;
    Animation animation;
} PlayerEntity;

typedef enum { FRIGHT, CHASE, SCATTER } GhostState;

typedef struct {
    int w;
    int h;
    SDL_Point pos;
    SDL_Point gridPos;
    SDL_Rect hitbox;
    enum Directions currMove;
    SDL_Point target;
    bool eaten;
    u32 sinceHome;   // time since in home
    SDL_Texture *texture;
    SDL_Point texturePos;
    Animation animation;
} GhostEntity;

typedef struct {
    int width;
    int height;
    int *data;
    u8 *points;
    int rectCount;
    SDL_Point portals[2];
    SDL_Point gate[2];
    SDL_Texture *texture;
    SDL_Rect *rects;
} Map;

typedef struct {
    u32 ticks;
    PlayerEntity *player;
    Map *map;
    GhostEntity *ghosts[GHOST_NUMBER];
    int score;
    SDL_Texture *powerPointTexture;
    GhostState ghostState;
    SDL_Rect ghostHome;
    u32 sinceFright;   // time since in fright mode
    // universal ghost textures
    SDL_Texture *frightenedTexture;
    SDL_Texture *eatenTexture;
} Stage;

#endif   // WAKAWAKA_TYPES_H
