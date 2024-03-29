//
// Created by filip on 19.11.22.
//

#include "stage.h"
#include "common.h"
#include "ghost/ghost.h"
#include "map.h"
#include "render.h"
#include "text.h"
#include "util.h"

#define ghosts stage.ghosts

extern App app;
extern Stage stage;

static void logic(void);

static void render(void);

static void initUniversalTextures(void);

static void initPlayer(void);

/*static void initGhosts(void);*/

static void initMap(void);

static void renderPlayer(void);

static void renderGhosts(void);

/*static SDL_bool checkCollisions(void);*/

static void renderMap(void);

static void handlePlayer(void);

static void handleGhosts(void);

static void renderHUD(void);

static bool checkHome(int i);

static PlayerEntity *player;
static Map *map;
// we want pacmam to start centered
// grid position can't be centered
// so when he goes left at the start we decrement gridPos.x additionally
// we don't want that, not elegant but will work
static bool startMove;

/**
 * 0 - blinky
 * 1 - pinky
 * 2 - inky
 * 3 - clyde
 **/

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.render = render;

    memset(&stage, 0, sizeof(Stage));

    stage.player = player;
    initPlayer();

    initGhosts();
    stage.ghostState = CHASE;

    stage.map = map;
    initMap();
    stage.score = 0;
    initUniversalTextures();
    stage.ghostHome = (SDL_Rect){258, 392, 160, 90};

    initFonts();
}

static void initUniversalTextures(void) {
    stage.powerPointTexture = loadTexture("assets/power_point.png");
    stage.frightenedTexture = loadTexture("assets/fright.png");
    stage.eatenTexture = loadTexture("assets/eaten.png");
}

static void initPlayer(void) {
    player = malloc(sizeof(PlayerEntity));
    memset(player, 0, sizeof(PlayerEntity));
    stage.player = player;
    startMove = true;
    player->lives = 4;

    player->w = PLAYER_SIZE;
    player->h = PLAYER_SIZE;

    // starting player position (texture basically)
    player->x = PLAYER_START_X * TILE_SIZE + 4;
    player->y = OFFSET_TOP + PLAYER_START_Y * TILE_SIZE - 7;

    // starting player map grid position
    player->gridPos.x = PLAYER_START_X;
    player->gridPos.y = PLAYER_START_Y;

    player->currMove = UNDF;
    player->nextMove = UNDF;

    // starting center player point
    player->center = (SDL_Point){.x = PLAYER_START_X * TILE_SIZE,
                                 .y = OFFSET_TOP + PLAYER_START_Y * TILE_SIZE +
                                      TILE_SIZE / 2};

    player->rect = (SDL_Rect){.w = TILE_SIZE,
                              .h = TILE_SIZE,
                              .x = PLAYER_START_X * TILE_SIZE + TILE_SIZE / 2,
                              .y = OFFSET_TOP + PLAYER_START_Y * TILE_SIZE + 1};

    player->clydeRect =
        (SDL_Rect){.w = TILE_SIZE * 16,
                   .h = TILE_SIZE * 16,
                   .x = PLAYER_START_X * TILE_SIZE - TILE_SIZE * 7,
                   .y = PLAYER_START_Y * TILE_SIZE - TILE_SIZE * 4};

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

static void checkCollisions(void) {
    for (int i = 0; i < GHOST_NUMBER; i++) {
        if (SDL_HasIntersection(&ghosts[i]->hitbox, &player->rect)) {
            if (stage.ghostState == FRIGHT)
                ghosts[i]->eaten = true;
            else {
                player->currMove = UNDF;
                player->nextMove = UNDF;
                for (int j = 0; j < GHOST_NUMBER; j++)
                    ghosts[j]->currMove = UNDF;
            }
        }
    }
}

static void logic(void) {
    handlePlayer();
    handleGhosts();
    checkCollisions();
}

// static int checkGridPos(int x, int y) { return !map->data[map->width * y +
// x]; }
static int checkGridPos(int ghID, int x, int y) {
    // if gate and inside home => go through
    if (ghID != -1 &&
        ((x == map->gate[0].x && y == map->gate[0].y) ||
         (x == map->gate[1].x && y == map->gate[1].y)) &&
        checkHome(ghID)) {
        return 1;
    }
    return !map->data[map->width * y + x];
}

static bool checkTileCenter(SDL_Point *entityCenter) {
    return (entityCenter->x - (TILE_SIZE / 2)) % TILE_SIZE == 0 &&
           ((entityCenter->y - OFFSET_TOP) - (TILE_SIZE / 2)) % TILE_SIZE == 0;
}

static void checkPoints(void) {
    u8 *tile = &map->points[player->gridPos.y * map->width + player->gridPos.x];
    if (*tile == 1) {
        stage.score += 10;
        *tile = 0;
    } else if (*tile == 2) {
        stage.score += 50;
        *tile = 0;
        stage.ghostState = FRIGHT;
        stage.sinceFright = stage.ticks;
    }
}

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
        if (checkTileCenter(&player->center)) {
            // we are in center of TILE, we can try change direction
            // first we must count the grid move we just made
            switch (player->currMove) {
            case LEFT:
                if (!startMove)
                    player->gridPos.x--;
                else
                    startMove = false;
                // check left portal
                if (cmpPoints(player->gridPos, map->portals[0])) {
                    player->gridPos = map->portals[1];
                    player->x = (map->portals[1].x - 1) * TILE_SIZE - 7;
                    player->y = OFFSET_TOP + map->portals[1].y * TILE_SIZE - 7;
                    player->center.x =
                        (map->portals[1].x - 1) * TILE_SIZE + TILE_SIZE / 2;
                    player->center.y = OFFSET_TOP +
                                       map->portals[1].y * TILE_SIZE +
                                       TILE_SIZE / 2;
                    player->rect.x = (map->portals[1].x - 1) * TILE_SIZE + 1;
                    player->rect.y =
                        OFFSET_TOP + map->portals[1].y * TILE_SIZE + 1;
                    return;
                }
                updateTargets(-4, 0);
                break;
            case DOWN:
                player->gridPos.y++;
                updateTargets(0, 4);
                break;
            case UP:
                player->gridPos.y--;
                updateTargets(-4, -4);
                break;
            case RIGHT:
                if (startMove)
                    startMove = false;
                player->gridPos.x++;
                // check right portal
                if (cmpPoints(player->gridPos, map->portals[1])) {
                    player->gridPos = map->portals[0];
                    player->x = (map->portals[0].x + 1) * TILE_SIZE - 7;
                    player->y = OFFSET_TOP + map->portals[0].y * TILE_SIZE - 7;
                    player->center.x =
                        (map->portals[0].x + 1) * TILE_SIZE + TILE_SIZE / 2;
                    player->center.y = OFFSET_TOP +
                                       map->portals[0].y * TILE_SIZE +
                                       TILE_SIZE / 2;
                    player->rect.x = (map->portals[0].x + 1) * TILE_SIZE + 1;
                    player->rect.y =
                        OFFSET_TOP + map->portals[0].y * TILE_SIZE + 1;
                    return;
                }
                updateTargets(4, 0);
                break;
            case UNDF:
                break;
            }
            //
            checkPoints();
            // if we want to change direction at nearest TILE center
            // if (player->nextMove != player->currMove) {
            // try changing directioin
            // TODO: rozdělit check nextMove a currMove
            // pokud se liší pacman se nezastaví před stenou
            switch (player->nextMove) {
            case LEFT:
                if (checkGridPos(-1, player->gridPos.x - 1,
                                 player->gridPos.y)) {
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
                if (checkGridPos(-1, player->gridPos.x,
                                 player->gridPos.y + 1)) {
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
                if (checkGridPos(-1, player->gridPos.x,
                                 player->gridPos.y - 1)) {
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
                if (checkGridPos(-1, player->gridPos.x + 1,
                                 player->gridPos.y)) {
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

        u32 animationSpeed = stage.ticks / 100;
        u32 idx = animationSpeed % 4;

        switch (player->currMove) {
        case LEFT:
            player->x -= PLAYER_SPEED;
            player->center.x -= PLAYER_SPEED;
            player->rect.x -= PLAYER_SPEED;
            player->clydeRect.x -= PLAYER_SPEED;
            player->animation.clip = player->animation.leftClips[idx];
            break;
        case DOWN:
            player->y += PLAYER_SPEED;
            player->center.y += PLAYER_SPEED;
            player->rect.y += PLAYER_SPEED;
            player->clydeRect.y += PLAYER_SPEED;
            player->animation.clip = player->animation.downClips[idx];
            break;
        case UP:
            player->y -= PLAYER_SPEED;
            player->center.y -= PLAYER_SPEED;
            player->rect.y -= PLAYER_SPEED;
            player->clydeRect.y -= PLAYER_SPEED;
            player->animation.clip = player->animation.upClips[idx];
            break;
        case RIGHT:
            player->x += PLAYER_SPEED;
            player->center.x += PLAYER_SPEED;
            player->rect.x += PLAYER_SPEED;
            player->clydeRect.x += PLAYER_SPEED;
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
}

// get tiles around (ugly don't look)
//   2
// 0 X 3
//   1
// TODO: move this somewhere else
static void getTilesAround(SDL_Point *tiles, SDL_Point target) {
    tiles[0].x = target.x - 1;
    tiles[0].y = target.y;
    tiles[1].x = target.x;
    tiles[1].y = target.y + 1;
    tiles[2].x = target.x;
    tiles[2].y = target.y - 1;
    tiles[3].x = target.x + 1;
    tiles[3].y = target.y;
}

static int getDistance(SDL_Point a, SDL_Point b) {
    int x = (a.x - b.x) * (a.x - b.x);
    int y = (a.y - b.y) * (a.y - b.y);
    return (int) sqrt(x + y);
}

static enum Directions checkTiles(int ghID, SDL_Point *tiles, SDL_Point target,
                                  int turnAround) {
    int minDist = 100;
    enum Directions res = UNDF;
    for (int i = 0; i < 4; i++) {
        if (i == turnAround) {
            continue;
        }
        int dist = getDistance(tiles[i], target);
        if (checkGridPos(ghID, tiles[i].x, tiles[i].y) && minDist >= dist) {
            minDist = dist;
            res = (enum Directions) i;
        }
    }
    return res;
}

// left ghost portal
static void doLeftPortalG(int i) {
    ghosts[i]->gridPos = map->portals[1];
    // -1 aby se nepřičetl grid pos :)
    ghosts[i]->pos.x = map->portals[1].x * TILE_SIZE + TILE_SIZE / 2 - 1;
    ghosts[i]->pos.y =
        OFFSET_TOP + map->portals[1].y * TILE_SIZE + TILE_SIZE / 2;
    ghosts[i]->texturePos.x = map->portals[1].x * TILE_SIZE - 7 - 1;
    ghosts[i]->texturePos.y = OFFSET_TOP + map->portals[1].y * TILE_SIZE - 7;
    ghosts[i]->hitbox.x = ghosts[i]->texturePos.x;
    ghosts[i]->hitbox.y = ghosts[i]->texturePos.y;
}

static void doRightPortalG(int i) {
    ghosts[i]->gridPos = map->portals[0];
    // +1 aby se nepřičetl grid pos :)
    ghosts[i]->pos.x = map->portals[0].x * TILE_SIZE + TILE_SIZE / 2 + 1;
    ghosts[i]->pos.y =
        OFFSET_TOP + map->portals[0].y * TILE_SIZE + TILE_SIZE / 2;
    ghosts[i]->texturePos.x = map->portals[0].x * TILE_SIZE - 7 + 1;
    ghosts[i]->texturePos.y = OFFSET_TOP + map->portals[0].y * TILE_SIZE - 7;
    ghosts[i]->hitbox.x = ghosts[i]->texturePos.x;
    ghosts[i]->hitbox.y = ghosts[i]->texturePos.y;
}

static bool isInFrontOfGate(SDL_Point gridPos) {
    if ((gridPos.x == map->gate[0].x && gridPos.y == map->gate[0].y - 1) ||
        (gridPos.x == map->gate[1].x && gridPos.y == map->gate[1].y - 1))
        return true;
    return false;
}

static bool checkHome(int i) {
    if (SDL_HasIntersection(&stage.ghostHome, &ghosts[i]->hitbox))
        return true;
    return false;
}

static void moveToHome(int i) {
    ghosts[i]->gridPos.y += 2;
    // ghosts[i]->pos.x -= 1;
    ghosts[i]->pos.y += 2 * TILE_SIZE;
    // ghosts[i]->texturePos.x -= 1;
    ghosts[i]->texturePos.y += 2 * TILE_SIZE;
    // ghosts[i]->hitbox.x = ghosts[i]->texturePos.x;
    ghosts[i]->hitbox.y = ghosts[i]->texturePos.y;
    ghosts[i]->currMove = UNDF;
}

static void chaseTarget(int i) {
    // decide next big brain move 🧠
    SDL_Point tilesAround[4];
    switch (ghosts[i]->currMove) {
    case LEFT:
        ghosts[i]->gridPos.x--;
        // check left portal
        if (cmpPoints(ghosts[i]->gridPos, map->portals[0])) {
            doLeftPortalG(i);
            return;
        }
        // if eaten and in front of gate go through
        if (ghosts[i]->eaten && isInFrontOfGate(ghosts[i]->gridPos)) {
            moveToHome(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = checkTiles(i, tilesAround, ghosts[i]->target, 3);
        if (i == 0)   // on blinky movement update inky target
            updateInkyTarget(-2, 0);
        else if (i == 3)
            updateClydeTarget();
        break;
    case DOWN:
        ghosts[i]->gridPos.y++;
        // if eaten and in front of gate go through
        if (ghosts[i]->eaten && isInFrontOfGate(ghosts[i]->gridPos)) {
            moveToHome(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = checkTiles(i, tilesAround, ghosts[i]->target, 2);
        if (i == 0)
            updateInkyTarget(0, 2);
        else if (i == 3)
            updateClydeTarget();
        break;
    case UP:
        ghosts[i]->gridPos.y--;
        // if eaten and in front of gate go through
        if (ghosts[i]->eaten && isInFrontOfGate(ghosts[i]->gridPos)) {
            moveToHome(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = checkTiles(i, tilesAround, ghosts[i]->target, 1);
        if (i == 0)
            updateInkyTarget(-2, -2);
        else if (i == 3)
            updateClydeTarget();
        break;
    case RIGHT:
        ghosts[i]->gridPos.x++;
        // check right portal
        if (cmpPoints(ghosts[i]->gridPos, map->portals[1])) {
            doRightPortalG(i);
            return;
        }
        // if eaten and in front of gate go through
        if (ghosts[i]->eaten && isInFrontOfGate(ghosts[i]->gridPos)) {
            moveToHome(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = checkTiles(i, tilesAround, ghosts[i]->target, i);
        if (i == 0)
            updateInkyTarget(2, 0);
        else if (i == 3)
            updateClydeTarget();
        break;
    case UNDF:
        break;
    }
}

static enum Directions chooseRngTile(SDL_Point *tiles, int turnAround) {
    int possibleDirections[3];
    int j = 0;   // keeping track of possible directions
    // select possible directions
    for (int i = 0; i < 4; i++) {
        if (i == turnAround)
            continue;
        if (checkGridPos(-1, tiles[i].x, tiles[i].y)) {
            possibleDirections[j] = i;
            j++;
        }
    }
    return (enum Directions) possibleDirections[getRngNum(0, j - 1)];
}

static void getFrightened(int i) {
    SDL_Point tilesAround[4];
    switch (ghosts[i]->currMove) {
    case LEFT:
        ghosts[i]->gridPos.x--;
        // check left portal
        if (cmpPoints(ghosts[i]->gridPos, map->portals[0])) {
            doLeftPortalG(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = chooseRngTile(tilesAround, 3);
        break;
    case DOWN:
        ghosts[i]->gridPos.y++;
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = chooseRngTile(tilesAround, 2);
        break;
    case UP:
        ghosts[i]->gridPos.y--;
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = chooseRngTile(tilesAround, 1);
        break;
    case RIGHT:
        ghosts[i]->gridPos.x++;
        // check right portal
        if (cmpPoints(ghosts[i]->gridPos, map->portals[1])) {
            doRightPortalG(i);
            return;
        }
        getTilesAround(tilesAround, ghosts[i]->gridPos);
        ghosts[i]->currMove = chooseRngTile(tilesAround, 0);
        break;
    case UNDF:
        break;
    }
    if ((stage.ticks - stage.sinceFright) > 8000) {
        stage.sinceFright = 0;
        stage.ghostState = CHASE;
        // bruh
        for (int j = 0; j < GHOST_NUMBER; j++)
            ghosts[j]->eaten = false;
    }
}

static void ghostHeal(int i) {
    if (ghosts[i]->sinceHome == 0)
        ghosts[i]->sinceHome = stage.ticks;
    else if ((stage.ticks - ghosts[i]->sinceHome) > 3000) {
        ghosts[i]->sinceHome = 0;
        ghosts[i]->eaten = false;
        ghosts[i]->currMove = UP;
    }
}

static void handleGhost(int i) {
    // animation shit
    u32 animationSpeed = stage.ticks / 100;
    u32 idx = animationSpeed % 2;

    switch (ghosts[i]->currMove) {
    case LEFT:
        ghosts[i]->animation.clip = ghosts[i]->animation.leftClips[idx];
        // printf("haf");
        break;
    case DOWN:
        ghosts[i]->animation.clip = ghosts[i]->animation.downClips[idx];
        break;
    case UP:
        ghosts[i]->animation.clip = ghosts[i]->animation.upClips[idx];
        break;
    case RIGHT:
        ghosts[i]->animation.clip = ghosts[i]->animation.rightClips[idx];
        break;
    case UNDF:
        break;
    }

    if (checkTileCenter(&ghosts[i]->pos)) {
        // do scatter and chase
        if (stage.ghostState != FRIGHT)
            chaseTarget(i);
        // fright & eaten & inside home
        else if (stage.ghostState == FRIGHT && ghosts[i]->eaten &&
                 checkHome(i)) {
            ghostHeal(i);
        }
        // fright & eaten
        else if (stage.ghostState == FRIGHT && ghosts[i]->eaten) {
            ghosts[i]->target = (SDL_Point){13, 14};
            chaseTarget(i);
        }
        // fright
        else if (stage.ghostState == FRIGHT)
            getFrightened(i);
    }

    // move the ghost
    switch (ghosts[i]->currMove) {
    case LEFT:
        ghosts[i]->pos.x -= PLAYER_SPEED;
        ghosts[i]->texturePos.x -= PLAYER_SPEED;
        ghosts[i]->hitbox.x -= PLAYER_SPEED;
        break;
    case DOWN:
        ghosts[i]->pos.y += PLAYER_SPEED;
        ghosts[i]->texturePos.y += PLAYER_SPEED;
        ghosts[i]->hitbox.y += PLAYER_SPEED;
        break;
    case UP:
        ghosts[i]->pos.y -= PLAYER_SPEED;
        ghosts[i]->texturePos.y -= PLAYER_SPEED;
        ghosts[i]->hitbox.y -= PLAYER_SPEED;
        break;
    case RIGHT:
        ghosts[i]->pos.x += PLAYER_SPEED;
        ghosts[i]->texturePos.x += PLAYER_SPEED;
        ghosts[i]->hitbox.x += PLAYER_SPEED;
        break;
    case UNDF:
        break;
    }
}

static void handleGhosts(void) {
    for (int i = 0; i < GHOST_NUMBER; i++) {
        handleGhost(i);
    }
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
    renderHUD();
    renderPlayer();
    renderGhosts();
}

static void renderPlayer(void) {
    renderClip(player->texture, &player->animation.clip, player->x, player->y);
    if (DEBUG) {
        setColor(0, 255, 0, 255);
        renderRectDiagonals(&player->rect);
        setColor(PEACH, 255);
        renderRectDiagonals(&player->clydeRect);
        setColor(RED, 255);
        renderRectDiagonals(&stage.ghostHome);
        printf("%d %d\n", ghosts[0]->gridPos.x, ghosts[0]->gridPos.y);
    }
}

static void renderGhosts(void) {
    setColor(BLUE, 255);
    for (int i = 0; i < GHOST_NUMBER; i++) {
        if (stage.ghostState == CHASE || stage.ghostState == SCATTER) {
            renderClip(ghosts[i]->texture, &ghosts[i]->animation.clip,
                       ghosts[i]->texturePos.x, ghosts[i]->texturePos.y);
        } else if (ghosts[i]->eaten) {
            renderClip(stage.eatenTexture, &ghosts[i]->animation.clip,
                       ghosts[i]->texturePos.x, ghosts[i]->texturePos.y);
        } else if (stage.ghostState == FRIGHT) {
            renderClip(stage.frightenedTexture, &ghosts[i]->animation.clip,
                       ghosts[i]->texturePos.x, ghosts[i]->texturePos.y);
        }
        if (DEBUG)
            renderRectDiagonals(&ghosts[i]->hitbox);
    }
    if (DEBUG) {
        setColor(RED, 255);
        renderGridRect(ghosts[0]->target);
        setColor(PINK, 255);
        renderGridRect(ghosts[1]->target);
        setColor(TEAL, 255);
        renderGridRect(ghosts[2]->target);
        setColor(PEACH, 255);
        renderGridRect(ghosts[3]->target);
    }
}

static void renderGamePoints(void) {
    u32 animationSpeed = stage.ticks / 250;
    for (int row = 0; row < map->height; row++) {
        for (int col = 0; col < map->width; col++) {
            u8 *tile = &map->points[row * map->width + col];
            if (*tile == 1)
                renderGamePoint(col, row);
            else if (*tile == 2 && animationSpeed % 2)
                renderPowerPoint(stage.powerPointTexture, col, row);
        }
    }
}

static void renderMap(void) {
    blit(map->texture, 0, OFFSET_TOP);
    renderGamePoints();
    /*if (DEBUG) {
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        for (int i = 0; i < map->rectCount; i++) {
            SDL_RenderDrawRect(app.renderer, &map->rects[i]);
        }
    }*/
}

static void renderHUD(void) {
    renderText((SCREEN_WIDTH / 2) - (5 * 35 / 2), 10, 255, 255, 255, "%s",
               "SCORE");
    renderText(300, 50, 255, 255, 255, "%d", stage.score);
    if (!app.gameStarted)
        renderText(SCREEN_WIDTH / 2 - 6 * 35 / 2 + 10, 495, 255, 255, 255, "%s",
                   "READY!");
}