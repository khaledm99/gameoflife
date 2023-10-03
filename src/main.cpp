#include "gameoflife.h"

int main()
{
    // initialize simulation
    GameOfLife g(432, 243);
    g.initDisplay();

    g.draw();

    // Main loop
    while(true)
    {
        g.next();
        g.draw();
        g.getInput();
        SDL_Delay(g.getSpeed());
    }
    return 0;
}
