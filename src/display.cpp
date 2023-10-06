#include "display.h"
#include <SDL2/SDL.h>


Display::Display(int windowWidth, int windowHeight, int gridWidth, int gridHeight)
{   
    setWin(windowWidth, windowHeight, 0, 0);
    setView(gridWidth, gridHeight, 0, 0);
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        throw(SDL_GetError());
    }

    setWindow( SDL_CreateWindow("GameOfLife",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,windowWidth,windowHeight,0));
    setRenderer(SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_TARGETTEXTURE));

    if(!(getWindow() && getRenderer()))
    {
        throw(SDL_GetError());
    }

    // No anti-aliasing
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    
    // Texture is a pixel grid with the world dimensions, scales up to the window size
    setTexture(SDL_CreateTexture(getRenderer(), SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, gridWidth, gridHeight));
    if(!getTexture())
    {
        throw(SDL_GetError());
    }

}
Display::~Display()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}


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
