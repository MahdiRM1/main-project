CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

all: program

program: main.o printmap.o generateArray.o updategame.o War.o
	$(CC) -o program main.o printmap.o generateArray.o updategame.o War.o $(LDFLAGS)

main.o: main.c printmap.h generateArray.h updategame.h
	$(CC) $(CFLAGS) -c main.c

printmap.o: printmap.c printmap.h
	$(CC) $(CFLAGS) -c printmap.c

generateArray.o: generateArray.c generateArray.h
	$(CC) $(CFLAGS) -c generateArray.c

updategame.o: updategame.c updategame.h
	$(CC) $(CFLAGS) -c updategame.c

War.o: War.c War.h
	$(CC) $(CFLAGS) -c War.c	

clean:
	rm -f *.o program
