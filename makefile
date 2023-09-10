gol: main.cpp gameoflife.cpp gameoflife.h
	g++ main.cpp -lSDL2 -o gol

clean:
	rm -f gol

