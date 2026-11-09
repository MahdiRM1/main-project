#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generateMap.h"
#include "raylib.h"

extern Tile map[17][17];
extern kingdom c[4];
extern int x, y;
extern int kingnum;
extern int alivePlayers;


int generate_number() {
    int random = rand() % 100 + 1;
    if(random < 70) return 1;
    else if(random < 85) return 2;
    else if(random < 95) return 3;
    else return 4;
}


void generate_array() {
    srand(time(NULL));
    int i, j;   
    for (i = 0; i < 17; i++) 
        for (j = 0; j < 17; j++) {
            if(i<x && j<y) {
                map[i][j].firstDiff = generate_number();
                map[i][j].type = TERRAIN;
            }
            else {
                map[i][j].type = EMPTY;
                map[i][j].firstDiff = INF;
            }
            map[i][j].forkingdom = -1;
            map[i][j].difficulty[0] = map[i][j].firstDiff;
            map[i][j].difficulty[1] = map[i][j].firstDiff;
            map[i][j].difficulty[2] = map[i][j].firstDiff;
            map[i][j].difficulty[3] = map[i][j].firstDiff;
            map[i][j].GoldRate = 0;
            map[i][j].FoodRate = 0;
        }
}