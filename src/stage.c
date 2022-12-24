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

    // starting player position
    player->x = 20;
    player->y = 20;

    // starting player map grid position
    player->gridPos.x = 1;
    player->gridPos.y = 1;

    player->currMove = UNDF;
    player->nextMove = UNDF;

    // starting center player point
    player->center = (SDL_Point){.x = TILE_SIZE + PLAYER_SIZE / 2,
                                 .y = TILE_SIZE + PLAYER_SIZE / 2};

    player->rect = (SDL_Rect){
        .w = PLAYER_SIZE, .h = PLAYER_SIZE, .x = TILE_SIZE, .y = TILE_SIZE};

    player->texture = loadTexture("assets/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void initMap(void) {
    map = map_load("assets/map");
    map_rectangles(map);

    map->texture = loadTexture("assets/map.png");
    SDL_QueryTexture(map->texture, NULL, NULL, &player->w, &player->h);
}

static void logic(void) { handlePlayer(); }

// TODO: util func
static int getGridPos(int x, int y) { return map->width * y + x; }

static void handlePlayer(void) {
    // player->dx = player->dy = 0;
    // player->rect.x = player->rect.x = 0;

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

    // try stopping player on center of every tile to see what happens

    if (player->nextMove != UNDF) {
        // plati i pro první tile
        if ((player->center.x - 15) % 30 == 0 &&
            (player->center.y - 15) % 30 == 0) {
            // we are in center of TILE, we can try change direction
            // first we must count the grid move we just made
            switch (player->currMove) {
            case LEFT:
                player->gridPos.x--;
                break;
            case DOWN:
                player->gridPos.y++;
                break;
            case UP:
                player->gridPos.y--;
                break;
            case RIGHT:
                player->gridPos.x++;
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
                if (!map->data[getGridPos(player->gridPos.x - 1,
                                          player->gridPos.y)]) {
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
                if (!map->data[getGridPos(player->gridPos.x,
                                          player->gridPos.y + 1)]) {
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
                if (!map->data[getGridPos(player->gridPos.x,
                                          player->gridPos.y - 1)]) {
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
                if (!map->data[getGridPos(player->gridPos.x + 1,
                                          player->gridPos.y)]) {
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

        switch (player->currMove) {
        case LEFT:
            player->x -= PLAYER_SPEED;
            player->center.x -= PLAYER_SPEED;
            player->rect.x -= PLAYER_SPEED;
            break;
        case DOWN:
            player->y += PLAYER_SPEED;
            player->center.y += PLAYER_SPEED;
            player->rect.y += PLAYER_SPEED;
            break;
        case UP:
            player->y -= PLAYER_SPEED;
            player->center.y -= PLAYER_SPEED;
            player->rect.y -= PLAYER_SPEED;
            break;
        case RIGHT:
            player->x += PLAYER_SPEED;
            player->center.x += PLAYER_SPEED;
            player->rect.x += PLAYER_SPEED;
            break;
        case UNDF:
            break;
        }
    }
    if (player->currMove == UNDF) {
        player->currMove = player->nextMove;
    }

    printf("grid position: [ %d, %d ]\n", player->gridPos.x, player->gridPos.y);
    // player->x += player->dx;
    // player->y += player->dy;
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
    blit(player->texture, player->x, player->y);
    SDL_SetRenderDrawColor(app.renderer, 250, 0, 0, 255);
    SDL_RenderFillRect(app.renderer, &player->rect);
    SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(app.renderer, player->center.x, player->center.y);
}

static void renderMap(void) {
    blit(map->texture, 0, 0);
    SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
    for (int i = 0; i < map->rectCount; i++) {
        SDL_RenderDrawRect(app.renderer, &map->rects[i]);
    }
}