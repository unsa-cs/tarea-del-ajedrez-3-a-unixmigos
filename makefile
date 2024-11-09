all: main.o chess.o gc.o
	g++ main.o chess.o gc.o $(HW).c -lglut -lGL -lGLU -w -o $(HW)
main.o: main.c chess.h
	g++ -c -g main.c
chess.o: chess.c chess.h figures.h
	g++ -c -g chess.c
gc.o: gc.c gc.h
	g++ -c -g gc.c
clean:
	rm *o prob*[^c]
