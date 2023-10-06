#include <SDL2/SDL.h>
#include <vector>
#include "gameoflife.h"
#include "util.h"
#include <stdlib.h>
#include <time.h>


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
    display = new Display(1280,720,width,height);
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
{
    delete display;
}




// Clear screen
// Default black background
int GameOfLife::clear()
{
    int checkError = 0;
    checkError += SDL_SetRenderTarget(display->getRenderer(), display->getTexture());
    checkError += SDL_SetRenderDrawColor(display->getRenderer(),0,0,0,255);
    checkError += SDL_RenderClear(display->getRenderer());
    checkError += SDL_SetRenderTarget(display->getRenderer(), NULL);

    checkError += SDL_RenderCopy(display->getRenderer(),display->getTexture(),display->getView(),display->getWin());
    if(checkError!=0) throw(SDL_GetError());
    return 0;
}

// Draw current teration
int GameOfLife::draw()
{
    int checkError = 0;
    try
    {
        clear();
    } catch (const char* &s){
        throw(s);
    }

    checkError+=SDL_SetRenderTarget(display->getRenderer(), display->getTexture());
    checkError+=SDL_SetRenderDrawColor(display->getRenderer(),255,255,255,255);   // White foreground

    // Draw the world. If world cell is 0, draw nothing, if 1, draw a white pixel
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(world[i][j]==1) checkError+=SDL_RenderDrawPoint(display->getRenderer(),j,i);
        }
    }

    // Green point at center of screen
    checkError+=SDL_SetRenderDrawColor(display->getRenderer(),0,255,0,255);
    checkError+=SDL_RenderDrawPoint(display->getRenderer(),width/2,height/2);

    // When paused, draw a grey cell wherever the mouse pointer is located
    if(paused)
    {
        drawCursor();
    }
    
    // Draw newly computed texture to window
    checkError+=SDL_SetRenderTarget(display->getRenderer(), NULL);
    checkError+=SDL_RenderCopy(display->getRenderer(),display->getTexture(),display->getView(),display->getWin());
    SDL_RenderPresent(display->getRenderer());
    
    if(checkError!=0) throw(SDL_GetError());
    return 0;
}

int GameOfLife::drawCursor()
{
    int checkError = 0;
    int x, y;
    SDL_GetMouseState(&x,&y);
    checkError+=SDL_SetRenderDrawColor(display->getRenderer(),200,200,200,255);
    checkError+=SDL_RenderDrawPoint(display->getRenderer(), (find_nearest(x,find_nearest(display->getWin()->w,display->getView()->w)))+display->getView()->x,(find_nearest(y,find_nearest(display->getWin()->h,display->getView()->h)))+display->getView()->y);
    if(checkError!=0) throw(SDL_GetError());
    return 0;
}

// Compute next iteration
int GameOfLife::next()
{
    // Count how many of the cells 8 neighbours are alive->
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
    SDL_Event* e;
    while(SDL_PollEvent(e = display->getEvent()))
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

void GameOfLife::setCell(int x, int y)
{
    world[y][x] = 1;
}
void GameOfLife::eraseCell(int x, int y)
{
    world[y][x] = 0;
}
int GameOfLife::getCell(int x, int y)
{
    return world[y][x];
}




