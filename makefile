CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

all: program

program: main.o printmap.o generateArray.o updategame.o
	$(CC) -o program main.o printmap.o generateArray.o updategame.o $(LDFLAGS)

main.o: main.c printmap.h generateArray.h updategame.h
	$(CC) $(CFLAGS) -c main.c

printmap.o: printmap.c printmap.h
	$(CC) $(CFLAGS) -c printmap.c

generateArray.o: generateArray.c generateArray.h
	$(CC) $(CFLAGS) -c generateArray.c

updategame.o: updategame.c updategame.h
	$(CC) $(CFLAGS) -c updategame.c

clean:
	rm -f *.o program
