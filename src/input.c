//
// Created by filip on 18.11.22.
//

#include "common.h"
#include "input.h"

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

void handleKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 1;
    }
}

void handleKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        app.keyboard[event->keysym.scancode] = 0;
    }
}