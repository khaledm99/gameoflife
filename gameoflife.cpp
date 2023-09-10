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
    //height = 90;
    //width = 160;
    height = 243;
    width = 432;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("GameOfLife",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,wwidth,wheight,0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

    win.x = win.y = 0;
    win.w = wwidth;
    win.h = wheight;

    view.x = view.y = 0;
    view.w = width;
    view.h = height;

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

    
    SDL_RenderCopy(renderer,texture,&view,&win);
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

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderDrawPoint(renderer,width/2,height/2);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer,texture,&view,&win);
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
                        case SDLK_p:
                            paused = !paused;
                            while(paused)
                            {
                                getInput();
                            }
                                
                            break;
                        case SDLK_n:
                            if(paused)
                            {
                                next();
                                draw();
                            }
                            break;
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
                        case SDLK_1:
                            SDL_Log("Zoom Level 1");
                            view.w = width;
                            view.h = height;
                            view.x = 0;
                            view.y = 0;
                            draw();
                            break;
                        case SDLK_2:
                            SDL_Log("Zoom Level 2");
                            view.w = 272;
                            view.h = 153;
                            view.x = (width-(view.w))/2;
                            view.y = (height-(view.h))/2;
                            draw();
                            break;
                        case SDLK_3:
                            SDL_Log("Zoom Level 3");
                            view.w = 144;
                            view.h = 81;
                            view.x = (width-(view.w))/2;
                            view.y = (height-(view.h))/2;
                            draw();
                            break;
                    }
                }
                break;
        }
    }
    
    return 0;
}




