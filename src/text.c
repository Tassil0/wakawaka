//
// Created by tassilo on 1/3/23.
//

#include "common.h"

#include "render.h"
#include "text.h"

static SDL_Texture *fontTexture;
static char drawTextBuffer[MAX_LINE_LENGTH];

void initFonts(void) { fontTexture = loadTexture("assets/font.png"); }

void renderText(int x, int y, int r, int g, int b, char *format, ...) {
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    rect.w = GLYPH_SIZE;
    rect.h = GLYPH_SIZE;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

    for (i = 0; i < len; i++) {
        c = drawTextBuffer[i];

        if (c >= ' ' && c <= 'Z') {
            rect.x = (c - ' ') * GLYPH_SIZE;

            blitRect(fontTexture, &rect, x, y);

            x += GLYPH_SIZE + 5;
        }
    }
}