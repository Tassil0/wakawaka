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

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("assets/player.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void initMap(void) {
    map = map_load("assets/map");
    map_rectangles(map);
}

static void logic(void) { handlePlayer(); }

static void handlePlayer(void) {
    player->dx = player->dy = 0;

    if (app.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }
    if (app.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }
    if (app.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }
    if (app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }

    player->x += player->dx;
    player->y += player->dy;
}

static void render(void) {
    renderPlayer();
    renderMap();
}

static void renderPlayer(void) { blit(player->texture, player->x, player->y); }

static void renderMap(void) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);
    for (int i = 0; i < map->rectCount; i++) {
        SDL_RenderDrawRect(app.renderer, &map->rects[i]);
    }
}