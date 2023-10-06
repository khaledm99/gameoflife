#include "input.h"
#include <SDL2/SDL.h>

void Input::setGamePtr(GameOfLife* game)
{
    g = game;
}
GameOfLife* Input::getGamePtr()
{
    return g;
}
    
ClickInput::ClickInput(int xcoord, int ycoord, int set)
{
    if(xcoord<0 || ycoord<0 || set<0 || set>1) throw ("Invalid click parameters");
    x = xcoord;
    y = ycoord;
    val = set;
}

void ClickInput::doAction() 
{

    if(g->getPaused()) 
    {
        if(val == 1) g->setCell(x,y);
        else g->eraseCell(x,y);
    }
}

void QuitInput::doAction() 
{
    g->quitFlag = true;
} 

void TogglePause::doAction() 
{
    g->setPaused(!g->getPaused());
    
}

void StepInput::doAction() 
{
    if(g->getPaused())
    {
        g->next();
    }
}

ChangeSpeedInput::ChangeSpeedInput(int delta)
{
    val = delta;
}

void ChangeSpeedInput::doAction() 
{
    g->changeSpeed(val);
}

void ResetInput::doAction() 
{
    for(int i=0; i< g->getHeight(); i++)
    {
        for(int j=0; j< g->getWidth(); j++)
        {
            if(rand() % 100 + 1 > 70) g->setCell(j,i);
        }
    }
}



