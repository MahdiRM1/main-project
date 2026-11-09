CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 

all: program

program: main.o printmap.o generateMap.o updategame.o War.o gameInformation.o info.o Computer.o
	$(CC) -o program main.o printmap.o generateMap.o updategame.o War.o gameInformation.o info.o Computer.o $(LDFLAGS)

main.o: main.c printmap.h generateMap.h updategame.h
	$(CC) $(CFLAGS) -c main.c

printmap.o: printmap.c printmap.h
	$(CC) $(CFLAGS) -c printmap.c

generateMap.o: generateMap.c generateMap.h
	$(CC) $(CFLAGS) -c generateMap.c

updategame.o: updategame.c updategame.h
	$(CC) $(CFLAGS) -c updategame.c

War.o: War.c War.h
	$(CC) $(CFLAGS) -c War.c	

gameInformation.o: gameInformation.c gameInformation.h
	$(CC) $(CFLAGS) -c gameInformation.c

info.o: info.c info.h
	$(CC) $(CFLAGS) -c info.c

Computer.o: Computer.c Computer.h
	$(CC) $(CFLAGS) -c Computer.c

clean:
	rm -f *.o program
