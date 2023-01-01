//
// Created by filip on 18.11.22.
//

#include <SDL2/SDL.h>

#ifndef WAKAWAKA_TYPES_H
#define WAKAWAKA_TYPES_H

#include "config.h"
#include <inttypes.h>

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
    SDL_Texture *texture;
    Animation animation;
} PlayerEntity;

typedef struct {
    int w;
    int h;
    SDL_Point pos;
    SDL_Point gridPos;
    SDL_Rect hitbox;
    enum Directions currMove;
    SDL_Point target;
    SDL_Texture *texture;
    SDL_Point texturePos;
    Animation animation;
} GhostEntity;

/*typedef enum { Frightened, Chase, Scatter } GhostSates;

typedef struct GhostState *GhostStatePtr;
typedef void (*EventStartFunc)(GhostStatePtr);
typedef void (*EventStopFunc)(GhostStatePtr);

struct GhostState {
    EventStartFunc start;
    EventStopFunc stop;
};*/

typedef struct {
    int width;
    int height;
    int *data;
    int rectCount;
    SDL_Texture *texture;
    SDL_Rect *rects;
} Map;

typedef struct {
    u32 ticks;
    PlayerEntity *player;
    Map *map;
    GhostEntity *(*ghosts)[GHOST_NUMBER];   // TODO: this is retarded
} Stage;

#endif   // WAKAWAKA_TYPES_H
