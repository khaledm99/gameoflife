#include <SDL2/SDL.h>
#include <vector>
#include "gameoflife.h"
#include <stdlib.h>
#include <time.h>



int mod_floor(int a, int n) {
    if(a<0) return n-1;
    else if(a==n) return 0;
    else return a;
    
}

int GameOfLife::init()
{
    height = 90;
    width = 160;
    wheight = 720;
    wwidth = 1280;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GameOfLife",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wwidth,wheight,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

    srand(time(NULL));

    world = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));
    temp = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));

    speed = 100;

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(rand() % 100 + 1 > 50) world[i][j] = 1;
        }
    }
    //world[10][10]=1;
    //world[8][11]=1;
    //world[10][11]=1;
    //world[10][12]=1;
    //world[9][12]=1;
    return 0;
} 

int GameOfLife::clear()
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect win;
    win.x = win.y = 0;
    win.w = 1280;
    win.h = 720;
    SDL_RenderCopy(renderer,texture,NULL,&win);
    return 0;
}

int GameOfLife::draw()
{
    clear();

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            if(world[i][j]==1) SDL_RenderDrawPoint(renderer,j,i);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect win;
    win.x = win.y = 0;
    win.w = 1280;
    win.h = 720;
    SDL_RenderCopy(renderer,texture,NULL,&win);
    SDL_RenderPresent(renderer);
    return 0;
}

int GameOfLife::next()
{
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

            if(world[i][j]==1)
            {
                if(sum<2||sum>3) temp[i][j]=0;
                else temp[i][j]=1;
            }
            if(world[i][j]==0 && sum==3) temp[i][j]=1; 
                
        }
    }
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            world[i][j] = temp[i][j];
        }
    }
    return 0;
}
        
int GameOfLife::getInput()
{    
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                SDL_Log("Program quit...");
                exit(0);
                break;
            case SDL_KEYDOWN:
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_RIGHTBRACKET:
                            speed-=20;
                            if(speed<0) speed=0;
                            SDL_Log("delay=%d",speed);
                            break;
                        case SDLK_LEFTBRACKET:
                            speed+=20;
                            if(speed>500) speed=500;
                            SDL_Log("delay=%d",speed);
                            break;
                        case SDLK_r:
                            for(int i=0; i<height; i++)
                            {
                                for(int j=0; j<width; j++)
                                {
                                    if(rand() % 100 + 1 > 50) world[i][j] = 1;
                                }
                            }
                            SDL_Log("randomized world");
                            break;
                        case SDLK_q:
                            SDL_Log("Program quit(q)...");
                            exit(0);
                            break;

                    }
                }
                break;
        }
    }
    
    return 0;
}




