#pragma once
#include "gameoflife.h"
class GameOfLife;

class Input
{
    protected:
        GameOfLife* g = nullptr;

    public:
        virtual ~Input(){}
        void setGamePtr(GameOfLife* game);
        GameOfLife* getGamePtr();
        virtual void doAction(){} 
};

class ClickInput : public Input
{
    public:
        int x, y, val;
        ClickInput(int xcoord, int ycoord, int set);
        void doAction() override;
};


class QuitInput : public Input
{
    public:
        void doAction() override;
};

class ChangeSpeedInput : public Input
{
    public:
        int val;
        ChangeSpeedInput(int delta);
        void doAction() override;
};


class TogglePause : public Input
{
    public:
        void doAction() override;
};

class StepInput : public Input
{
    public:
        void doAction() override;
};

class ResetInput : public Input
{
    public:
        void doAction() override;
};
