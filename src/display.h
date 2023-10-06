#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Display
{
    private:
        SDL_Event e;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture * texture = nullptr;
        SDL_Rect win;
        SDL_Rect view;
    public:
        Display(int windowWidth, int windowHeight, int gridWidth, int gridHeight);
        ~Display();

        void clear();
        void draw(std::vector<std::vector<int>> &grd);
        void drawPoint(int x, int y);
        void drawCursor();

        void getInput();

        SDL_Event* getEvent();
        SDL_Window* getWindow();
        void setWindow(SDL_Window* win);
        SDL_Renderer* getRenderer();
        void setRenderer(SDL_Renderer* ren);
        SDL_Texture* getTexture();
        void setTexture(SDL_Texture* tex);
        SDL_Rect* getWin();
        SDL_Rect* getView();
        void setWin(int w, int h, int x, int y);
        void setView(int w, int h, int x, int y);
};

