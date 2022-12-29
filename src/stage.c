//
// Created by filip on 19.11.22.
//

#include "stage.h"
#include "common.h"
#include "map.h"
#include "render.h"

extern App app;
extern Stage stage;

static void logic(void);

static void render(void);

static void initPlayer(void);

static void initMap(void);

static void renderPlayer(void);

/*static SDL_bool checkCollisions(void);*/

static void renderMap(void);

static void handlePlayer(void);

static PlayerEntity *player;
static Map *map;

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.render = render;

    memset(&stage, 0, sizeof(Stage));

    stage.player = player;
    initPlayer();

    stage.map = map;
    initMap();
}

static void initPlayer(void) {
    player = malloc(sizeof(PlayerEntity));
    memset(player, 0, sizeof(PlayerEntity));
    stage.player = player;

    player->w = PLAYER_SIZE;
    player->h = PLAYER_SIZE;

    // starting player position
    player->x = PLAYER_START_X * TILE_SIZE - 12;
    player->y = PLAYER_START_Y * TILE_SIZE - 12;

    // starting player map grid position
    player->gridPos.x = PLAYER_START_X;
    player->gridPos.y = PLAYER_START_Y;

    player->currMove = UNDF;
    player->nextMove = UNDF;

    // starting center player point
    player->center =
        (SDL_Point){.x = PLAYER_START_X * TILE_SIZE + TILE_SIZE / 2,
                    .y = PLAYER_START_Y * TILE_SIZE + TILE_SIZE / 2};

    player->rect = (SDL_Rect){.w = TILE_SIZE,
                              .h = TILE_SIZE,
                              .x = PLAYER_START_X * TILE_SIZE + 1,
                              .y = PLAYER_START_Y * TILE_SIZE + 1};

    player->texture = loadTexture("assets/player.png");

    player->animation.leftClips[0] = (SDL_Point){.x = 2 * PLAYER_SIZE, .y = 0};
    player->animation.leftClips[1] = (SDL_Point){.x = PLAYER_SIZE, .y = 0};
    player->animation.leftClips[2] = (SDL_Point){.x = 0, .y = 0};
    player->animation.leftClips[3] = (SDL_Point){.x = PLAYER_SIZE, .y = 0};

    player->animation.downClips[0] = (SDL_Point){.x = 2 * PLAYER_SIZE, .y = 0};
    player->animation.downClips[1] =
        (SDL_Point){.x = PLAYER_SIZE, .y = PLAYER_SIZE};
    player->animation.downClips[2] = (SDL_Point){.x = 0, .y = PLAYER_SIZE};
    player->animation.downClips[3] =
        (SDL_Point){.x = PLAYER_SIZE, .y = PLAYER_SIZE};

    player->animation.upClips[0] = (SDL_Point){.x = 2 * PLAYER_SIZE, .y = 0};
    player->animation.upClips[1] =
        (SDL_Point){.x = PLAYER_SIZE, .y = 2 * PLAYER_SIZE};
    player->animation.upClips[2] = (SDL_Point){.x = 0, .y = 2 * PLAYER_SIZE};
    player->animation.upClips[3] =
        (SDL_Point){.x = PLAYER_SIZE, .y = 2 * PLAYER_SIZE};

    player->animation.rightClips[0] = (SDL_Point){.x = 2 * PLAYER_SIZE, .y = 0};
    player->animation.rightClips[1] =
        (SDL_Point){.x = PLAYER_SIZE, .y = 3 * PLAYER_SIZE};
    player->animation.rightClips[2] = (SDL_Point){.x = 0, .y = 3 * PLAYER_SIZE};
    player->animation.rightClips[3] =
        (SDL_Point){.x = PLAYER_SIZE, .y = 3 * PLAYER_SIZE};

    player->animation.clip = player->animation.leftClips[0];
}

static void initMap(void) {
    map = map_load("assets/map");
    map_rectangles(map);

    map->texture = loadTexture("assets/map.png");
    SDL_QueryTexture(map->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void) { handlePlayer(); }

static int checkGridPos(int x, int y) { return !map->data[map->width * y + x]; }

static void handlePlayer(void) {
    if (app.keyboard[SDL_SCANCODE_UP]) {
        player->nextMove = UP;
    } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
        player->nextMove = DOWN;
    } else if (app.keyboard[SDL_SCANCODE_LEFT]) {
        player->nextMove = LEFT;
    } else if (app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->nextMove = RIGHT;
    } else if (app.keyboard[SDL_SCANCODE_SPACE]) {
        player->nextMove = UNDF;
    }

    if (player->nextMove != UNDF) {
        // plati i pro první tile
        if ((player->center.x - (TILE_SIZE / 2)) % TILE_SIZE == 0 &&
            (player->center.y - (TILE_SIZE / 2)) % TILE_SIZE == 0) {
            // we are in center of TILE, we can try change direction
            // first we must count the grid move we just made
            switch (player->currMove) {
            case LEFT:
                player->gridPos.x--;
                /*if (!checkGridPos(player->gridPos.x - 1, player->gridPos.y)) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                }*/
                break;
            case DOWN:
                player->gridPos.y++;
                /*if (!checkGridPos(player->gridPos.x, player->gridPos.y + 1)) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                }*/
                break;
            case UP:
                player->gridPos.y--;
                /* if (!checkGridPos(player->gridPos.x, player->gridPos.y - 1))
                 { player->currMove = UNDF; player->nextMove = UNDF;
                 }*/
                break;
            case RIGHT:
                player->gridPos.x++;
                /*if (!checkGridPos(player->gridPos.x + 1, player->gridPos.y)) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                }*/
                break;
            case UNDF:
                break;
            }
            // if we want to change direction at nearest TILE center
            // if (player->nextMove != player->currMove) {
            // try changing directioin
            // TODO: rozdělit check nextMove a currMove
            // pokud se liší pacman se nezastaví před stenou
            switch (player->nextMove) {
            case LEFT:
                if (checkGridPos(player->gridPos.x - 1, player->gridPos.y)) {
                    player->currMove = player->nextMove;
                } else if (player->nextMove == player->currMove) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                } else {
                    // player->currMove = UNDF;
                    player->nextMove = UNDF;
                }
                break;
            case DOWN:
                if (checkGridPos(player->gridPos.x, player->gridPos.y + 1)) {
                    player->currMove = player->nextMove;
                } else if (player->nextMove == player->currMove) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                } else {
                    // player->currMove = UNDF;
                    player->nextMove = UNDF;
                }
                break;
            case UP:
                if (checkGridPos(player->gridPos.x, player->gridPos.y - 1)) {
                    player->currMove = player->nextMove;
                } else if (player->nextMove == player->currMove) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                } else {
                    // player->currMove = UNDF;
                    player->nextMove = UNDF;
                }
                break;
            case RIGHT:
                if (checkGridPos(player->gridPos.x + 1, player->gridPos.y)) {
                    player->currMove = player->nextMove;
                } else if (player->nextMove == player->currMove) {
                    player->currMove = UNDF;
                    player->nextMove = UNDF;
                } else {
                    // player->currMove = UNDF;
                    player->nextMove = UNDF;
                }
                break;
            case UNDF:
                break;
            }
            //}
        }

        u32 animationSpeed = SDL_GetTicks() / 100;
        u32 idx = animationSpeed % 4;

        switch (player->currMove) {
        case LEFT:
            player->x -= PLAYER_SPEED;
            player->center.x -= PLAYER_SPEED;
            player->rect.x -= PLAYER_SPEED;
            player->animation.clip = player->animation.leftClips[idx];
            break;
        case DOWN:
            player->y += PLAYER_SPEED;
            player->center.y += PLAYER_SPEED;
            player->rect.y += PLAYER_SPEED;
            player->animation.clip = player->animation.downClips[idx];
            break;
        case UP:
            player->y -= PLAYER_SPEED;
            player->center.y -= PLAYER_SPEED;
            player->rect.y -= PLAYER_SPEED;
            player->animation.clip = player->animation.upClips[idx];
            break;
        case RIGHT:
            player->x += PLAYER_SPEED;
            player->center.x += PLAYER_SPEED;
            player->rect.x += PLAYER_SPEED;
            player->animation.clip = player->animation.rightClips[idx];
            break;
        case UNDF:
            player->animation.clip = player->animation.leftClips[0];
            break;
        }
    }
    if (player->currMove == UNDF) {
        player->currMove = player->nextMove;
    }

    printf("grid position: [ %d, %d ]\n", player->gridPos.x, player->gridPos.y);
}

/*static SDL_bool checkCollisions(void) {
    int j = 0;
    SDL_Rect res;
    for (int i = 0; i < map->rectCount; i++) {
        if (!SDL_IntersectRect(&player->rect, &map->rects[i], &res)) {
            j++;
        }
    }
    if (j == map->rectCount) {
        return SDL_TRUE;
    }
    return SDL_FALSE;
}*/

static void render(void) {
    renderMap();
    renderPlayer();
}

static void renderPlayer(void) {
    renderClip(player->texture, &player->animation.clip, player->x, player->y);
    if (DEBUG) {
        SDL_SetRenderDrawColor(app.renderer, 250, 0, 0, 255);
        // SDL_RenderFillRect(app.renderer, &player->rect);
        SDL_RenderDrawRect(app.renderer, &player->rect);
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        // SDL_RenderDrawPoint(app.renderer, player->center.x,
        // player->center.y);
        renderDiagonals(&player->rect);
    }
}

static void renderMap(void) {
    blit(map->texture, 0, 0);
    /*if (DEBUG) {
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        for (int i = 0; i < map->rectCount; i++) {
            SDL_RenderDrawRect(app.renderer, &map->rects[i]);
        }
    }*/
}