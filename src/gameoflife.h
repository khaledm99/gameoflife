#ifndef GOL_H
#define GOL_H

#include <SDL2/SDL.h>
#include <vector>

// Handles the simulation logic of Conway's Game of Life
// Directly uses SDL2 for graphics, input handling, and
// window management.
// TODO: 
//  Massive refactoring
class GameOfLife {

    private:
        int height;         // Dimensions of simulation grid
        int width;
        int wheight = 720;  // Dimensions of window, grid is scaled to fit
        int wwidth =1280;
        bool paused = false;   

        // SDL boilerplate
        SDL_Event e;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Rect win;       // Used to pass dimensions information to window object
        SDL_Rect view;      // Used to pass dimensions information to renderer - to change zoom level

        
        std::vector<std::vector<int>> world;    // 2D grid of cells
        std::vector<std::vector<int>> temp;     // Temporary grid for updating world

    public:
        int speed;                              // Speed of simulation
        int init();                             // Initializes SDL, simulation
        int clear();                            // Clears screen
        int draw();                             // Draws current iteration of world
        int next();                             // Computes next iteration
        int getInput();                         // Handles keyboard input

        static int find_nearest(unsigned int a, unsigned int b);
        static int mod_floor(int a, int n);

};

#endif
