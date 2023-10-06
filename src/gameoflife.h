#ifndef GOL_H
#define GOL_H

#include <SDL2/SDL.h>
#include <vector>
#include "display.h"

class Display;

// Handles the simulation logic of Conway's Game of Life
// Directly uses SDL2 for graphics, input handling, and
// window management.
// TODO: 
//  Massive refactoring
class GameOfLife {

    private:
        int height;         // Dimensions of simulation grid
        int width;
        int speed;
        const int MAX_SPEED = 500;
        int wheight = 720;  // Dimensions of window, grid is scaled to fit
        int wwidth =1280;
        bool paused = false;   

        Display* display = nullptr;
        
        std::vector<std::vector<int>> world;    // 2D grid of cells
        std::vector<std::vector<int>> temp;     // Temporary grid for updating world

    public:
        int iteration=0;
        bool quitFlag = false;
        int changeSpeed(int amount);
        int getSpeed();
        int getHeight();
        int getWidth();
        std::vector<std::vector<int>> getWorld();
        void setWorld(std::vector<std::vector<int>> arr);

        void setDisplay(Display* d);
        Display* getDisplay();
        
        GameOfLife(int w, int h);               // Constructor
        ~GameOfLife();

        void setCell(int x, int y);
        void eraseCell(int x, int y);
        int getCell(int x, int y);
        bool getPaused();
        void setPaused(bool p); 

        int draw();                             // Draws current iteration of world
        int next();                             // Computes next iteration
        void getInput();                         // Handles keyboard input
        void delay();


};

#endif
