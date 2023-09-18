# Conway's Game Of Life

![](https://github.com/khaledm99/gameoflife/blob/220ebfdeb2c77bed7233ef5d9127ae5356ae9171/Game%20of%20Life%20Demo.gif)

This is an interactive gameoflife simulator. Requires SDL2.

What started as a simple exercise using a simple game of life implementation to learn SDL2 for graphics,

this project continues to evolve as I use it to improve my programming skills and learn new frameworks.

Run 'make' to compile, run 'gol' to start the simulation.

The sim is currently a single window with an empty grid. Controls are:

(r) - Randomly populate grid

(1),(2),(3) - change zoom from furthest to nearest, respectively

([) - slow down simulation

(]) - speed up simulation

(n) - advance simulation by 1 step

(q) - quit

(p) - pause simulation

While paused, a cursor will appear wherever your mouse cursor is located. 

All normal controls work while paused, as well as additional pause only controls:

(Left Click) - Place a cell

(Right Click) - Erase a cell

