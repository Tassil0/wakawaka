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

    while (1) {
        prepareScene();

        handleInput();

        app.delegate.logic();
        app.delegate.render();

        presentScene();

        SDL_Delay(16);
    }

    return 0;
}