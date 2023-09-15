#ifndef DISP_H
#define DISP_H

#include <SDL2/SDL.h>
#include <vector>

class Display {
    public:
        int wheight = 720;
        int wwidth = 1280;
        SDL_Event e;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Rect win;
        SDL_Rect view;

        int init(int game_w, int game_h);
        int clear();
        int present();
        int drawCell(int x, int y);
        int targetTexture();
        int targetWindow();
        int drawMouseCursor();
};

#endif
