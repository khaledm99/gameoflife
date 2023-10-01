#include "gameoflife.h"

int main()
{
    // initialize simulation
    GameOfLife g;
    g.init();

    g.draw();

    // Main loop
    while(true)
    {
        g.next();
        g.draw();
        g.getInput();
        SDL_Delay(g.speed);
    }
    return 0;
}
