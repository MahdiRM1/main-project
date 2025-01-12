#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "generateArray.h"
#include "raylib.h"

extern Tile map[17][17];

int generate_number() {//randomize road difficulty
    const double probs [9] = {0.65, 0.25, 0.05, 0.05, 0, 0, 0, 0, 0};
    double cdf [9];
    cdf [0] = probs [0];
    for (int i=1; i < 9; ++i) {
        cdf [i] = cdf [i-1] + probs[i];
    }
    double rand_val = (double) rand() / RAND_MAX;
    for (int i=0; i<9; ++i) {
        if (rand_val < cdf[i]) return (char)(i + 1);
    }
    return -1;
}

void generate_array(int x, int y) {
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
        }

}