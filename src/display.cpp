#include "display.h"
#include "util.h"
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

// Clear screen
// Default black background
void Display::clear()
{
    int checkError = 0;
    checkError += SDL_SetRenderTarget(renderer, texture);
    checkError += SDL_SetRenderDrawColor(renderer,0,0,0,255);
    checkError += SDL_RenderClear(renderer);
    checkError += SDL_SetRenderTarget(renderer, NULL);

    checkError += SDL_RenderCopy(renderer,texture,&view,&win);
    if(checkError!=0) throw(SDL_GetError());
}

// Draw current teration
void Display::drawPoint(int x, int y)
{
    int checkError = 0;
    checkError+=SDL_SetRenderTarget(renderer, texture);
    checkError+=SDL_SetRenderDrawColor(renderer,255,255,255,255);   // White foreground
    checkError+=SDL_RenderDrawPoint(renderer,x,y);
    checkError+=SDL_RenderDrawPoint(renderer,win.w/2,win.h/2);
    if(checkError!=0) throw(SDL_GetError());
}
void Display::draw(std::vector<std::vector<int>> &grid)
{
    int checkError = 0;
    try
    {
        clear();
    } catch (const char* s){
        throw(s);
    }

    // Draw the world. If world cell is 0, draw nothing, if 1, draw a white pixel
    for(int i=0; i<grid.size(); i++)
    {
        for(int j=0; j<grid[0].size(); j++)
        {
            if(grid[i][j]==1) drawPoint(j,i);
        }
    }

    // Green point at center of screen
    //checkError+=SDL_SetRenderDrawColor(renderer,0,255,0,255);
    //checkError+=SDL_RenderDrawPoint(renderer,win.w/2,win.h/2);
    
    // Draw newly computed texture to window
    checkError+=SDL_SetRenderTarget(renderer, NULL);
    checkError+=SDL_RenderCopy(renderer,texture,&view,&win);
    SDL_RenderPresent(renderer);
    
    if(checkError!=0) throw(SDL_GetError());
}

void Display::drawCursor()
{
    int x, y;
    SDL_GetMouseState(&x,&y);
    drawPoint((find_nearest(x,find_nearest(win.w,view.w)))+view.x,(find_nearest(y,find_nearest(win.h,view.h)))+view.y);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer,texture,&view,&win);
    SDL_RenderPresent(renderer);

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

// Handle input using SDl
void Display::getInput()
{    
    while(SDL_PollEvent(&e)
    {
        switch(e->type)
        {
            // X button on window
            case SDL_QUIT:
                SDL_Log("Program quit...");
                exit(0);
                break;

            // Mouse click
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch(e->button.button)
                    {
                        // Left click when paused will place a cell at the mouse cursor
                        case SDL_BUTTON_LEFT:
                            if(paused)
                            {
                                world[(find_nearest(e->button.y,find_nearest(display->getWin()->h,display->getView()->h)))+display->getView()->y][(find_nearest(e->button.x,find_nearest(display->getWin()->w,display->getView()->w)))+display->getView()->x] = 1;
                            }
                            break;

                        // Right click when paused will delete a cell at the mouse cursor
                        case SDL_BUTTON_RIGHT:
                            if(paused)
                            {
                                world[(find_nearest(e->button.y,find_nearest(display->getWin()->h,display->getView()->h)))+display->getView()->y][(find_nearest(e->button.x,find_nearest(display->getWin()->w,display->getView()->w)))+display->getView()->x] = 0;
                            }
                            break;
                    }
                }
                break;
            // keyboard events
            case SDL_KEYDOWN:
                {
                    switch(e->key.keysym.sym)
                    {

                        // p will toggle pause state->
                        // While paused, sim will continue to draw and 
                        // handle input
                        case SDLK_p:
                            paused = !paused;
                            while(paused)
                            {
                                draw();
                                getInput();
                            }
                            break;
                        // n will compute next iteration when paused
                        case SDLK_n:
                            if(paused)
                            {
                                next();
                            }
                            break;
                        // Brackets will decrease and increase sim speed, respectively
                        // Speed is clamped to 0 and 500
                        case SDLK_RIGHTBRACKET:
                            changeSpeed(-20);
                            break;
                        case SDLK_LEFTBRACKET:
                            changeSpeed(20);
                            break;
                        // r will randomly populate world
                        case SDLK_r:
                            for(int i=0; i<height; i++)
                            {
                                for(int j=0; j<width; j++)
                                {
                                    if(rand() % 100 + 1 > 70) world[i][j] = 1;
                                }
                            }
                            SDL_Log("randomized world");
                            break;
                        // q will quit
                        case SDLK_q:
                            SDL_Log("Program quit(q)...");
                            exit(0);
                            break;
                        // number keys 1-3 will set the zoom level
                        case SDLK_1:
                            SDL_Log("Zoom Level 1");
                            display->setView(width,height,0,0);
                            draw();
                            break;
                        case SDLK_2:
                            SDL_Log("Zoom Level 2");
                            display->setView(272,153,(width-272)/2,(height-153)/2);
                            draw();
                            break;
                        case SDLK_3:
                            SDL_Log("Zoom Level 3");
                            display->setView(144,81,(width-144)/2,(height-81)/2);
                            draw();
                            break;
                    }
                }
                break;
        }
    }
    
    return 0;
}
