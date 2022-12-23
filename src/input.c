//
// Created by filip on 18.11.22.
//

#include "input.h"
#include "common.h"

extern App app;

void handleInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_KEYDOWN:
            handleKeyDown(&event.key);
            break;
        case SDL_KEYUP:
            handleKeyUp(&event.key);
            break;
        default:
            break;
        }
    }
}

void resetKeys(SDL_KeyboardEvent *event) {
    app.keyboard[SDL_SCANCODE_UP] = 0;
    app.keyboard[SDL_SCANCODE_DOWN] = 0;
    app.keyboard[SDL_SCANCODE_LEFT] = 0;
    app.keyboard[SDL_SCANCODE_RIGHT] = 0;
}

void handleKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        resetKeys(event);
        app.keyboard[event->keysym.scancode] = 1;
    }
}

void handleKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        // resetKeys();
        // app.keyboard[event->keysym.scancode] = 0;
    }
}