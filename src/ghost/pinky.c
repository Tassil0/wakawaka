//
// Created by tassilo on 12/29/22.
//

#include "ghost.h"

// starting position on grid
#define START_X 13
#define START_Y 13

extern Stage stage;

static GhostEntity *createPinky(void);

static GhostEntity *createPinky(void) {
    GhostEntity *pinky = malloc(sizeof(GhostEntity));
    memset(pinky, 0, sizeof(GhostEntity));
    return pinky;
}

GhostEntity *initPinky(void) {
    GhostEntity *pinky = createPinky();
    pinky->w = GHOST_SIZE;
    pinky->h = GHOST_SIZE;

    // TODO: center pos?
    pinky->pos.x = START_X * TILE_SIZE + TILE_SIZE / 2;
    pinky->pos.y = START_Y * TILE_SIZE + TILE_SIZE / 2;

    pinky->gridPos.x = START_X;
    pinky->gridPos.y = START_Y;

    pinky->currMove = LEFT;

    // TODO: load texture in stage

    pinky->texturePos.x = START_X * TILE_SIZE - 7;
    pinky->texturePos.y = START_Y * TILE_SIZE - 7;

    pinky->hitbox = (SDL_Rect){.x = pinky->texturePos.x,
                               .y = pinky->texturePos.y,
                               .w = GHOST_SIZE,
                               .h = GHOST_SIZE};

    pinky->animation.leftClips[0] = (SDL_Point){.x = 0, .y = 0};
    pinky->animation.leftClips[1] = (SDL_Point){.x = GHOST_SIZE, .y = 0};

    pinky->animation.downClips[0] = (SDL_Point){.x = 0, .y = GHOST_SIZE};
    pinky->animation.downClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = GHOST_SIZE};

    pinky->animation.upClips[0] = (SDL_Point){.x = 0, .y = 2 * GHOST_SIZE};
    pinky->animation.upClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 2 * GHOST_SIZE};

    pinky->animation.rightClips[0] = (SDL_Point){.x = 0, .y = 3 * GHOST_SIZE};
    pinky->animation.rightClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 3 * GHOST_SIZE};

    pinky->animation.clip = pinky->animation.leftClips[0];

    return pinky;
}

/* TODO:
void handlePinky(void) {
    GhostEntity *pinky = stage.
    switch (stage.ghosts[0]->currMove) {

    }
}*/
