#include "gameoflife.h"

int main()
{
    GameOfLife g;
    g.init();

    g.draw();
    while(true)
    {
        g.next();
        g.draw();
        g.getInput();
        SDL_Delay(g.speed);
    }
    return 0;
}
