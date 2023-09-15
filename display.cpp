#include "display.h"
#include "util.h"
#include <SDL2/SDL.h>


int Display::init(int game_w, int game_h)
{

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GameOfLife",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wwidth,wheight,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, game_w, game_h);

    win.x = win.y = 0;
    win.w = wwidth;
    win.h = wheight;

    view.x = view.y = 0;
    view.w = game_w;
    view.h = game_h;
    SDL_SetRenderTarget(renderer,texture);

    return 0;
}
int Display::clear()
{
    //SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    
    //present();
    return 0;
}

int Display::targetTexture()
{
    SDL_SetRenderTarget(renderer,texture);
    return 0;
}

int Display::targetWindow()
{
    SDL_SetRenderTarget(renderer,NULL);
    return 0;
}

int Display::drawCell(int x, int y)
{
    //SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDrawPoint(renderer, x, y);
    return 0;
}

int Display::drawMouseCursor()
{
    int x,y;
    //SDL_SetRenderTarget(renderer,texture);
    SDL_GetMouseState(&x,&y);
    SDL_RenderDrawPoint(renderer, (find_nearest(x,find_nearest(win.w,view.w)))+view.x,(find_nearest(y,find_nearest(win.h,view.h)))+view.y);
    
    return 0;
}

int Display::present()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer,texture,&view,&win);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer,texture);

    return 0;
}
