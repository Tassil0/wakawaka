//
// Created by filip on 13.11.22.
//

#include "main.h"
#include "common.h"
#include "init.h"
#include "input.h"
#include "render.h"
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
        stage.ticks = SDL_GetTicks();
        prepareScene();

        handleInput();

        app.delegate.logic();
        app.delegate.render();

        presentScene();

        // TODO: move this
        double avgFPS =
            (float) countedFrames / (float) (stage.ticks - startTime) * 1000.0;
        if (avgFPS > 2000000) {
            avgFPS = 0;
        }
        // printf("FPS: %f\n", avgFPS);

        SDL_Delay(6);
        ++countedFrames;

        // TODO: move this
        // wait for user input
        SDL_Event event;
        while (!app.gameStarted) {
            if (SDL_WaitEvent(&event)) {
                if (event.type == SDL_KEYDOWN)
                    app.gameStarted = true;
            }
        }
    }

    return 0;
}
// get tiles around (ugly don't look)
//   2
// 0 X 3
//   1
