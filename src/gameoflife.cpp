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

int GameOfLife::draw()
{
    display->draw(world); 
    // When paused, draw a grey cell wherever the mouse pointer is located
    if(paused)
    {
        display->drawCursor();
    }
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




