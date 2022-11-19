//
// Created by filip on 13.11.22.
//

#include "common.h"
#include "render.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "stage.h"

App app;
Stage stage;

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));

    initSDL();

    initStage();

    atexit(cleanup);

    int countedFrames = 0;
    unsigned int startTime = SDL_GetTicks();

    while (1) {
        prepareScene();

        handleInput();

        app.delegate.logic();
        app.delegate.render();

        presentScene();

        double avgFPS = (float) countedFrames / (float) (SDL_GetTicks() - startTime) * 1000.0;
        if (avgFPS > 2000000) {
            avgFPS = 0;
        }
        printf("FPS: %f\n", avgFPS);

        SDL_Delay(6);
        ++countedFrames;
    }

    return 0;
}