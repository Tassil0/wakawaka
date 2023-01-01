//
// Created by tassilo on 12/29/22.
//

#include "ghosts.h"

// starting position on grid
#define START_X 10   // 13
#define START_Y 20   // 11

extern Stage stage;

static GhostEntity *createBlinky(void);

static GhostEntity *createBlinky(void) {
    GhostEntity *blinky = malloc(sizeof(GhostEntity));
    memset(blinky, 0, sizeof(GhostEntity));
    return blinky;
}

GhostEntity *initBlinky(void) {
    GhostEntity *blinky = createBlinky();
    blinky->w = GHOST_SIZE;
    blinky->h = GHOST_SIZE;

    // TODO: center pos?
    blinky->pos.x = START_X * TILE_SIZE + TILE_SIZE / 2 - 1;
    blinky->pos.y = START_Y * TILE_SIZE + TILE_SIZE / 2;

    blinky->gridPos.x = START_X;
    blinky->gridPos.y = START_Y;

    blinky->currMove = LEFT;

    blinky->target.x = 12;
    blinky->target.y = 23;

    // TODO: load texture in stage

    blinky->texturePos.x = START_X * TILE_SIZE - 7 - 1;
    blinky->texturePos.y = START_Y * TILE_SIZE - 7;

    blinky->hitbox = (SDL_Rect){.x = blinky->texturePos.x,
                                .y = blinky->texturePos.y,
                                .w = GHOST_SIZE,
                                .h = GHOST_SIZE};

    blinky->animation.leftClips[0] = (SDL_Point){.x = 0, .y = 0};
    blinky->animation.leftClips[1] = (SDL_Point){.x = GHOST_SIZE, .y = 0};

    blinky->animation.downClips[0] = (SDL_Point){.x = 0, .y = GHOST_SIZE};
    blinky->animation.downClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = GHOST_SIZE};

    blinky->animation.upClips[0] = (SDL_Point){.x = 0, .y = 2 * GHOST_SIZE};
    blinky->animation.upClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 2 * GHOST_SIZE};

    blinky->animation.rightClips[0] = (SDL_Point){.x = 0, .y = 3 * GHOST_SIZE};
    blinky->animation.rightClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 3 * GHOST_SIZE};

    blinky->animation.clip = blinky->animation.leftClips[0];

    return blinky;
}

/* TODO:
void handleBlinky(void) {
    GhostEntity *blinky = stage.
    switch (stage.ghosts[0]->currMove) {

    }
}*/
