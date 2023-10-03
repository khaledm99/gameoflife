#include <SDL2/SDL.h>
#include <vector>
#include "gameoflife.h"
#include <stdlib.h>
#include <time.h>


// Helper function to compute a mod n,
// including the case where a is negative.
// C++ built in mod() function doesn't handle
// the negative case
int GameOfLife::mod_floor(int a, int n) {
    if(a<0) return n+a;
    else if(a==n) return 0;
    else if(a>n) return mod_floor(a-n, n);
    else return a;
    
}

// Helper function to round integer division
// to the nearest integer. C++ default behaviour is
// to truncate downwards.
int GameOfLife::find_nearest(unsigned int a, unsigned int b) {
    if(b==0) throw "divide by zero";
    return (a + (b/2)) / b;
    //return round((double)a/(double)b);
}


// Initialize simulation
GameOfLife::GameOfLife(int w, int h)
{
    if(w<1 || h<1) 
    {
        throw("Dimensions must be positive");
        exit(1);
    }
    height = h;   // Dimensions of grid
    width = w;

    srand(time(NULL));

    // Initialize simulation world variables to zero
    world = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));
    temp = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));
    
    // Initial speed 
    speed = 100;
} 

int GameOfLife::getHeight()
{
    return height;
}
int GameOfLife::getWidth()
{
    return width;
}
std::vector<std::vector<int>> GameOfLife::getWorld()
{
    return world;
}

GameOfLife::~GameOfLife()
{}

int GameOfLife::initDisplay()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GameOfLife",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wwidth,wheight,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

    // No anti-aliasing
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    
    // Texture is a pixel grid with the world dimensions, scales up to the window size
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

    setWin(wwidth, wheight, 0, 0);
    setView(width, height, 0, 0);

    return 0;
}

void GameOfLife::setView(int w, int h, int x, int y)
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
SDL_Rect GameOfLife::getView()
{
    return view;
}
SDL_Rect GameOfLife::getWin()
{
    return win;
}

void GameOfLife::setWin(int w, int h, int x, int y)
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

// Clear screen
// Default black background
int GameOfLife::clear()
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer,texture,&view,&win);
    return 0;
}

// Draw current teration
int GameOfLife::draw()
{
    clear();

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);   // White foreground

    // Draw the world. If world cell is 0, draw nothing, if 1, draw a white pixel
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(world[i][j]==1) SDL_RenderDrawPoint(renderer,j,i);
        }
    }

    // Green point at center of screen
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderDrawPoint(renderer,width/2,height/2);

    // When paused, draw a grey cell wherever the mouse pointer is located
    if(paused)
    {
        drawCursor();
    }
    
    // Draw newly computed texture to window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer,texture,&view,&win);
    SDL_RenderPresent(renderer);
    return 0;
}

int GameOfLife::drawCursor()
{
    int x, y;
    SDL_GetMouseState(&x,&y);
    SDL_SetRenderDrawColor(renderer,200,200,200,255);
    SDL_RenderDrawPoint(renderer, (find_nearest(x,find_nearest(win.w,view.w)))+view.x,(find_nearest(y,find_nearest(win.h,view.h)))+view.y);
    return 0;
}

// Compute next iteration
int GameOfLife::next()
{
    // Count how many of the cells 8 neighbours are alive.
    int sum=0; 
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            sum=0;
                sum+=world[mod_floor(i-1,height)][mod_floor(j-1,width)];
                sum+=world[(((i-1)%height)+height)%height][j];
                sum+=world[mod_floor(i-1,height)][mod_floor(j+1,width)];

                sum+=world[i][mod_floor(j-1,width)];
                sum+=world[i][mod_floor(j+1,width)];
                
                sum+=world[mod_floor(i+1,height)][mod_floor(j-1,width)];
                sum+=world[mod_floor(i+1,height)][j];
                sum+=world[mod_floor(i+1,height)][mod_floor(j+1,width)];

            /*
             * Conway's Game of Life Rules:
             * 1. If a cell is alive and has fewer than 2 neighbours, it dies
             * 2. If a cell is alive and has greater than 3 neighbours, it dies
             * 3. If a cell is dead and has exactly 3 neighbours, it comes to life
             * The computations are applied to the temporary world array
             */ 
            if(world[i][j]==1)
            {
                if(sum<2||sum>3) temp[i][j]=0;
                else temp[i][j]=1;
            }
            if(world[i][j]==0 && sum==3) temp[i][j]=1; 
                
        }
    }
    
    // Copy newly computed iteration to primary world array
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            world[i][j] = temp[i][j];
        }
    }
    return 0;
}
        
// Handle input using SDl
int GameOfLife::getInput()
{    
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            // X button on window
            case SDL_QUIT:
                SDL_Log("Program quit...");
                exit(0);
                break;

            // Mouse click
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch(e.button.button)
                    {
                        // Left click when paused will place a cell at the mouse cursor
                        case SDL_BUTTON_LEFT:
                            if(paused)
                            {
                                world[(find_nearest(e.button.y,find_nearest(win.h,view.h)))+view.y][(find_nearest(e.button.x,find_nearest(win.w,view.w)))+view.x] = 1;
                            }
                            break;

                        // Right click when paused will delete a cell at the mouse cursor
                        case SDL_BUTTON_RIGHT:
                            if(paused)
                            {
                                world[(find_nearest(e.button.y,find_nearest(win.h,view.h)))+view.y][(find_nearest(e.button.x,find_nearest(win.w,view.w)))+view.x] = 0;
                            }
                            break;
                    }
                }
                break;
            // keyboard events
            case SDL_KEYDOWN:
                {
                    switch(e.key.keysym.sym)
                    {

                        // p will toggle pause state.
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
                            setView(width,height,0,0);
                            draw();
                            break;
                        case SDLK_2:
                            SDL_Log("Zoom Level 2");
                            setView(272,153,(width-view.w)/2,(height-view.h)/2);
                            draw();
                            break;
                        case SDLK_3:
                            SDL_Log("Zoom Level 3");
                            setView(144,81,(width-view.w)/2,(height-view.h)/2);
                            draw();
                            break;
                    }
                }
                break;
        }
    }
    
    return 0;
}

int GameOfLife::changeSpeed(int amount)
{
    speed+=amount;
    if(speed<0) speed=0;
    if(speed>MAX_SPEED) speed=MAX_SPEED;
    return speed;
}
int GameOfLife::getSpeed()
{
    return speed;
}




