#include "display.h"
#include <SDL2/SDL.h>


SDL_Window* Display::getWindow()
{
    return window;
}
void Display::setWindow(SDL_Window * win)
{
    window = win;
}
SDL_Renderer* Display::getRenderer()
{
    return renderer;
}
void Display::setRenderer(SDL_Renderer* ren)
{
    renderer = ren;
}
SDL_Texture* Display::getTexture()
{
    return texture;
}
void Display::setTexture(SDL_Texture* tex)
{
    texture = tex;
}
void Display::setView(int w, int h, int x, int y)
{
    if(w<1 || h<1 || x<0 || y<0)
    {
        throw("Dimensions must be positive, origin must be nonnegative");
        return;
    }
    view.w = w;
    view.h = h;
    view.x = x;
    view.y = y;
}
SDL_Rect* Display::getView()
{
    return &view;
}
SDL_Rect* Display::getWin()
{
    return &win;
}

void Display::setWin(int w, int h, int x, int y)
{
    if(w<1 || h<1 || x<0 || y<0)
    {
        throw("Dimensions must be positive, origin must be nonnegative");
        return;
    }
    win.w = w;
    win.h = h;
    win.x = x;
    win.y = y;
}
SDL_Event* Display::getEvent()
{
    return &e;
}
