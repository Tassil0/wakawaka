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

static SDL_bool checkCollisions(void);

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

    player->x = 30;
    player->y = 30;

    player->rect =
        (SDL_Rect){.w = 25, .h = 25, .x = player->x + 15, .y = player->y + 15};

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

static void handlePlayer(void) {
    player->dx = player->dy = 0;
    // player->rect.x = player->rect.x = 0;

    if (checkCollisions()) {
        if (app.keyboard[SDL_SCANCODE_UP]) {
            player->dy = -PLAYER_SPEED;
            player->dx = 0;
            player->rect.y -= PLAYER_SPEED;
        } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
            player->dy = PLAYER_SPEED;
            player->dx = 0;
            player->rect.y += PLAYER_SPEED;
        } else if (app.keyboard[SDL_SCANCODE_LEFT]) {
            player->dx = -PLAYER_SPEED;
            player->dy = 0;
            player->rect.x -= PLAYER_SPEED;
        } else if (app.keyboard[SDL_SCANCODE_RIGHT]) {
            player->dx = PLAYER_SPEED;
            player->dy = 0;
            player->rect.x += PLAYER_SPEED;
        }
    } else {
        player->x--;
        player->x--;
        player->rect.x--;
        player->rect.y--;
    }

    player->x += player->dx;
    player->y += player->dy;
}

static SDL_bool checkCollisions(void) {
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
}

static void render(void) {
    renderMap();
    renderPlayer();
}

static void renderPlayer(void) {
    blit(player->texture, player->x, player->y);
    SDL_SetRenderDrawColor(app.renderer, 2, 250, 205, 255);
    SDL_RenderDrawRect(app.renderer, &player->rect);
}

static void renderMap(void) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);
    for (int i = 0; i < map->rectCount; i++) {
        SDL_RenderDrawRect(app.renderer, &map->rects[i]);
    }
    // blit(map->texture, 0, 0);
}