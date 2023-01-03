//
// Created by tassilo on 1/3/23.
//

#include "util.h"
#include "common.h"

int getRngNum(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}