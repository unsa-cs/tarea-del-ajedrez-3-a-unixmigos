all: main.o chess.o gc.o
	g++ main.o chess.o gc.o $(HW).c -lglut -lGL -lGLU -w -o $(HW)
main.o: main.c chess.h
	g++ -c main.c
chess.o: chess.c chess.h figures.h
	g++ -c chess.c
gc.o: gc.c gc.h
	g++ -c gc.c
clean:
	rm *o prob*[^c]
