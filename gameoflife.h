#ifndef GOL_H
#define GOL_H

#include <SDL2/SDL.h>
#include <vector>

class GameOfLife {

    private:
        int height;
        int width;
        int wheight = 720;
        int wwidth =1280;
        bool paused = false;

        SDL_Event e;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Rect win;
        SDL_Rect view;

        
        std::vector<std::vector<int>> world;
        std::vector<std::vector<int>> temp;

    public:
        int speed;
        int init();
        int clear();
        int draw();
        int next();
        int getInput();         

};

#endif
