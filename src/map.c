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
    map->portals[0] = (SDL_Point){.x = 1, .y = 15};
    map->portals[1] = (SDL_Point){.x = 27, .y = 15};
    map->gate[0] = (SDL_Point){.x = 13, .y = 14};
    map->gate[1] = (SDL_Point){.x = 14, .y = 14};
    return map;
}

Map *map_load(char *filename) {
    FILE *f = fopen(filename, "rt");
    if (f == NULL) {
        // free(f); TODO: WTF??
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not open map: %s",
                        filename);
        exit(1);
    }

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
    return map;
}

void map_rectangles(Map *map) {
    map->rects = malloc(map->rectCount * sizeof(SDL_Rect));
    int x = 0;
    int y = 0;
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
