#pragma once
#include <vector>
#include "input.h"

class Input;

class DisplayInterface
{
    public:
        virtual void clear(){}
        virtual void draw(std::vector<std::vector<int>> &grid){}
        virtual void drawPoint(int x, int y){}
        virtual void drawCursor(){}
        virtual void dWait(int delay){}

        virtual Input* getInput() = 0;
};

