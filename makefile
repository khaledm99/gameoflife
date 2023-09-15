gol: main.cpp display.cpp display.h gameoflife.cpp gameoflife.h
	g++ main.cpp gameoflife.cpp display.cpp -lSDL2 -o gol

clean:
	rm -f gol

