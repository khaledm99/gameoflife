#include "gameoflife.h"

int main()
{
    // initialize simulation
    const int WIDTH = 432;
    const int HEIGHT = 234;

    GameOfLife g(WIDTH, HEIGHT);
    g.setDisplay(new Display(1280,720,WIDTH,HEIGHT));

    //g.draw();

    // Main loop
    while(!g.quitFlag)
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
        while(g.getPaused())
        {
            g.draw();
            g.getInput();
        }
        SDL_Delay(g.getSpeed());
    }
    return 0;
}
