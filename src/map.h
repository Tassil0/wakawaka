//
// Created by tassilo on 12/20/22.
//

#ifndef WAKAWAKA_MAP_H
#define WAKAWAKA_MAP_H

#include "common.h"

Map *map_load(char *filename);
void map_rectangles(Map *map);
bool cmpPoints(SDL_Point a, SDL_Point b);

#endif   // WAKAWAKA_MAP_H
