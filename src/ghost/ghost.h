//
// Created by tassilo on 12/29/22.
//

#ifndef WAKAWAKA_GHOST_H
#define WAKAWAKA_GHOST_H

#include "../types.h"

void initGhosts(void);
void destroyGhost(void);
void updateTargets(int x, int y);
void updateInkyTarget(int x, int y);
void updateClydeTarget(void);

#endif   // WAKAWAKA_GHOST_H
