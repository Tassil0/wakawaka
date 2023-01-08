//
// Created by tassilo on 1/2/23.
//

#include "ghost.h"
#include "../render.h"

/*
 * 0 - shadow / blinky (red)
 * 1 - speedy / pinky (pink)
 * 2 - bashful / inky (blue)
 * 3 - pokey / clyde (yellow)
 */

extern Stage stage;

static int allStartX[4] = {13, 12, 14, 15};
static int allStartY[4] = {11, 14, 14, 14};
static int scatterTargetsX[4] = {26, 2, 27, 0};
static int scatterTargetsY[4] = {-1, -1, 31, 31};
static const char *textures[4] = {
    "assets/blinky.png",
    "assets/pinky.png",
    "assets/inky.png",
    "assets/clyde.png",
};

static GhostEntity *createGhost(void);

static GhostEntity *initGhost(int startX, int startY, const char *texture);

static SDL_Point findInkyTarget(int x, int y, SDL_Point blinkyGridPos);

static GhostEntity *createGhost(void) {
    GhostEntity *ghost = (GhostEntity *) malloc(sizeof(GhostEntity));
    memset(ghost, 0, sizeof(GhostEntity));
    return ghost;
}

static GhostEntity *initGhost(int startX, int startY, const char *texture) {
    GhostEntity *ghost = createGhost();
    ghost->w = GHOST_SIZE;
    ghost->h = GHOST_SIZE;

    // TODO: center pos?
    ghost->pos.x = startX * TILE_SIZE + TILE_SIZE / 2 - 1;
    ghost->pos.y = OFFSET_TOP + startY * TILE_SIZE + TILE_SIZE / 2;

    ghost->gridPos.x = startX;
    ghost->gridPos.y = startY;

    ghost->currMove = LEFT;

    ghost->target.x = 12;
    ghost->target.y = 23;

    ghost->sinceHome = 0;

    ghost->texture = loadTexture(texture);

    ghost->texturePos.x = startX * TILE_SIZE - 7 - 1;
    ghost->texturePos.y = OFFSET_TOP + startY * TILE_SIZE - 7;

    ghost->hitbox = (SDL_Rect){.x = ghost->texturePos.x,
                               .y = ghost->texturePos.y,
                               .w = GHOST_SIZE,
                               .h = GHOST_SIZE};

    ghost->animation.leftClips[0] = (SDL_Point){.x = 0, .y = 0};
    ghost->animation.leftClips[1] = (SDL_Point){.x = GHOST_SIZE, .y = 0};

    ghost->animation.downClips[0] = (SDL_Point){.x = 0, .y = GHOST_SIZE};
    ghost->animation.downClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = GHOST_SIZE};

    ghost->animation.upClips[0] = (SDL_Point){.x = 0, .y = 2 * GHOST_SIZE};
    ghost->animation.upClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 2 * GHOST_SIZE};

    ghost->animation.rightClips[0] = (SDL_Point){.x = 0, .y = 3 * GHOST_SIZE};
    ghost->animation.rightClips[1] =
        (SDL_Point){.x = GHOST_SIZE, .y = 3 * GHOST_SIZE};

    ghost->animation.clip = ghost->animation.leftClips[0];

    ghost->eaten = false;

    return ghost;
}

static SDL_Point findInkyTarget(int x, int y, SDL_Point blinkyGridPos) {
    // calc vector
    SDL_Point vector =
        (SDL_Point){.x = blinkyGridPos.x - x, .y = blinkyGridPos.y - y};
    // rotate vector
    vector.x = vector.x * (-1);
    vector.y = vector.y * (-1);
    return (SDL_Point){.x = x + vector.x, .y = y + vector.y};
}

void initGhosts(void) {
    for (int i = 0; i < GHOST_NUMBER; i++) {
        stage.ghosts[i] = initGhost(allStartX[i], allStartY[i], textures[i]);
    }
}

void updateTargets(int x, int y) {
    if (!stage.ghosts[0]->eaten)
        stage.ghosts[0]->target = stage.player->gridPos;
    if (!stage.ghosts[1]->eaten) {
        stage.ghosts[1]->target.x = stage.player->gridPos.x + x;
        stage.ghosts[1]->target.y = stage.player->gridPos.y + y;
    }
    if (stage.ghostState == SCATTER) {
        for (int i = 0; i < GHOST_NUMBER; i++)
            stage.ghosts[i]->target =
                (SDL_Point){.x = scatterTargetsX[i], .y = scatterTargetsY[i]};
    }
}

void updateInkyTarget(int x, int y) {
    if (stage.ghostState == CHASE && !stage.ghosts[2]->eaten) {
        stage.ghosts[2]->target = findInkyTarget(stage.player->gridPos.x + x,
                                                 stage.player->gridPos.y + y,
                                                 stage.ghosts[0]->gridPos);
    }
}

void updateClydeTarget(void) {
    if (SDL_HasIntersection(&stage.ghosts[3]->hitbox,
                            &stage.player->clydeRect) &&
        stage.ghostState == CHASE && !stage.ghosts[3]->eaten)
        stage.ghosts[3]->target = (SDL_Point){.x = 0, .y = 31};
    else if (!stage.ghosts[3]->eaten)
        stage.ghosts[3]->target = stage.player->gridPos;
}