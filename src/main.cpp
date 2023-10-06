#include "gameoflife.h"

int main()
{
    // initialize simulation
    GameOfLife g(432, 243);

    g.draw();

    // Main loop
    while(true)
    {
        g.next();
        try
        {
            g.draw();
        } catch (const char* s)
        { 
            printf(s);
            exit(1);
        }
        g.getInput();
        SDL_Delay(g.getSpeed());
    }
    return 0;
}
