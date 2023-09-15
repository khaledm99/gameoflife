#include <SDL2/SDL.h>
#include <vector>
#include "gameoflife.h"
#include "display.h"
#include "util.h"
#include <stdlib.h>
#include <time.h>





int GameOfLife::init()
{
    //height = 90;
    //width = 160;
    height = 243;
    width = 432;

 //   SDL_Init(SDL_INIT_VIDEO);

 //   disp.window = SDL_Createdisp.window("GameOfLife",SDL_disp.winDOWPOS_UNDEFINED,SDL_disp.winDOWPOS_UNDEFINED,wwidth,wheight,0);
 //   renderer = SDL_CreateRenderer(disp.window, -1, SDL_RENDERER_TARGETTEXTURE);

 //   SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
 //   texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

 //   disp.win.x = disp.win.y = 0;
 //   disp.win.w = wwidth;
 //   disp.win.h = wheight;

 //   disp.view.x = disp.view.y = 0;
 //   disp.view.w = width;
 //   disp.view.h = height;
 
    disp.init(width, height);

    srand(time(NULL));

    world = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));
    temp = std::vector<std::vector<int>> (height, std::vector<int> (width, 0));
    
    speed = 100;

    
    //world[10][10]=1;
    //world[8][11]=1;
    //world[10][11]=1;
    //world[10][12]=1;
    //world[9][12]=1;
    return 0;
} 

int GameOfLife::drawWorld()
{
    disp.clear();

    for(int i=0; i<world.size(); i++)
    {
        for(int j=0; j<world[0].size(); j++)
        {
            if(world[i][j]==1) disp.drawCell(j,i);
        }
    }
    if(paused)
    {
        disp.drawMouseCursor();
    }
    
    disp.present();
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

int GameOfLife::draw()
{
    drawWorld();
    return 0;
}
        
int GameOfLife::getInput()
{    
    while(SDL_PollEvent(&disp.e))
    {
        switch(disp.e.type)
        {
            case SDL_QUIT:
                SDL_Log("Program quit...");
                exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    switch(disp.e.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            if(paused)
                            {
                                world[(find_nearest(disp.e.button.y,find_nearest(disp.win.h,disp.view.h)))+disp.view.y][(find_nearest(disp.e.button.x,find_nearest(disp.win.w,disp.view.w)))+disp.view.x] = 1;
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            if(paused)
                            {
                                world[(find_nearest(disp.e.button.y,find_nearest(disp.win.h,disp.view.h)))+disp.view.y][(find_nearest(disp.e.button.x,find_nearest(disp.win.w,disp.view.w)))+disp.view.x] = 0;
                            }
                            break;
                    }
                }
                break;
            case SDL_KEYDOWN:
                {
                    switch(disp.e.key.keysym.sym)
                    {
                        case SDLK_p:
                            paused = !paused;
                            while(paused)
                            {
                                drawWorld();
                                getInput();
                            }
                            break;
                        case SDLK_n:
                            if(paused)
                            {
                                next();
                                //drawWorld();
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
                                    if(rand() % 100 + 1 > 70) world[i][j] = 1;
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
                            disp.view.w = width;
                            disp.view.h = height;
                            disp.view.x = 0;
                            disp.view.y = 0;
                            drawWorld();
                            break;
                        case SDLK_2:
                            SDL_Log("Zoom Level 2");
                            disp.view.w = 272;
                            disp.view.h = 153;
                            disp.view.x = (width-(disp.view.w))/2;
                            disp.view.y = (height-(disp.view.h))/2;
                            drawWorld();
                            break;
                        case SDLK_3:
                            SDL_Log("Zoom Level 3");
                            disp.view.w = 144;
                            disp.view.h = 81;
                            disp.view.x = (width-(disp.view.w))/2;
                            disp.view.y = (height-(disp.view.h))/2;
                            drawWorld();
                            break;
                    }
                }
                break;
        }
    }
    
    return 0;
}




