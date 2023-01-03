//
// Created by tassilo on 12/20/22.
//

#include "map.h"

#define RECT_WIDTH  TILE_SIZE
#define RECT_HEIGHT TILE_SIZE

static Map *map_create(int width, int height) {
    Map *map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;
    map->rectCount = 0;
    map->data = (int *) malloc(width * height * sizeof(int));
    map->points = (u8 *) malloc(width * height * sizeof(u8));
    map->portals[0] = (SDL_Point){.x = 1, .y = 14};
    map->portals[1] = (SDL_Point){.x = 26, .y = 14};
    map->gate[0] = (SDL_Point){.x = 13, .y = 12};
    map->gate[1] = (SDL_Point){.x = 14, .y = 12};
    return map;
}

static void checkFile(FILE *f, char *filename) {
    if (f == NULL) {
        // free(f); TODO: WTF??
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not open file: %s",
                        filename);
        exit(1);
    }
}

static void loadPoints(u8 *points, int w, int h) {
    char *filename = "assets/map_points";
    FILE *f = fopen(filename, "rt");
    checkFile(f, filename);

    char buffer[256];
    for (int row = 0; row < h; row++) {
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\n")] = 0;
        int col = 0;
        for (char *tile = strtok(buffer, " "); tile != NULL;
             tile = strtok(NULL, " ")) {
            u8 tileVal = atoi(tile);
            points[row * w + col] = tileVal;
            col++;
        }
    }
    fclose(f);
}

Map *map_load(char *filename) {
    FILE *f = fopen(filename, "rt");
    checkFile(f, filename);

    char buffer[256];
    fgets(buffer, sizeof(buffer), f);
    buffer[strcspn(buffer, "\n")] = 0;
    int width = atoi(strtok(buffer, " "));
    int height = atoi(strtok(NULL, " "));
    Map *map = map_create(width, height);
    for (int row = 0; row < height; row++) {
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\n")] = 0;
        int col = 0;
        for (char *tile = strtok(buffer, " "); tile != NULL;
             tile = strtok(NULL, " ")) {
            int tileVal = atoi(tile);
            map->data[row * width + col] = tileVal;
            if (tileVal == 1) {
                map->rectCount++;
            }
            col++;
        }
    }
    fclose(f);

    loadPoints(map->points, width, height);
    return map;
}

void map_rectangles(Map *map) {
    map->rects = malloc(map->rectCount * sizeof(SDL_Rect));
    int x = 0;
    int y = OFFSET_TOP;
    int i = 0;
    for (int row = 0; row < map->height; row++) {
        for (int col = 0; col < map->width; col++) {
            if (map->data[row * map->width + col] == 1) {
                map->rects[i].w = RECT_WIDTH;
                map->rects[i].h = RECT_HEIGHT;
                map->rects[i].x = x;
                map->rects[i].y = y;
                i++;
            }
            x += RECT_WIDTH;
        }
        y += RECT_HEIGHT;
        x = 0;
    }
}

bool cmpPoints(SDL_Point a, SDL_Point b) {
    if (a.x == b.x && a.y == b.y)
        return true;
    return false;
}