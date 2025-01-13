#include <stdio.h>
#include "updategame.h"
#include "generateArray.h"
#include "printmap.h"
#include "raylib.h"
#include "War.h"

extern kingdom c[4];
extern int kingnum;
extern Tile map[17][17];
extern int x, y;

int checkWar(int gridX, int gridY, int player, int *pl2) {
    int sw = 0;
    if      (map[gridX + 1][gridY].forkingdom != player && map[gridX + 1][gridY].forkingdom != -1){ gridX++; sw = 1;}
    else if (map[gridX][gridY + 1].forkingdom != player && map[gridX][gridY + 1].forkingdom != -1){ gridY++; sw = 1;}
    else if (map[gridX][gridY - 1].forkingdom != player && map[gridX][gridY - 1].forkingdom != -1){ gridY--; sw = 1;}
    else if (map[gridX - 1][gridY].forkingdom != player && map[gridX - 1][gridY].forkingdom != -1){ gridX--; sw = 1;}
    if(sw) *pl2 = map[gridX][gridY].forkingdom;
    if(sw && map[gridX][gridY].type==KINGDOM) return 2;
    return sw;
}

void DestroyLoser(int player){
    int i, j;
    for (i = 0; i < x; i++) 
        for (j = 0; j < y; j++) {
            if(map[i][j].forkingdom == player && map[i][j].type != KINGDOM) {
                map[i][j].forkingdom = -1;
                if(map[i][j].type == VILLAGE){
                    c[player].FoodRate -= map[i][j].FoodRate;
                    c[player].GoldRate -= map[i][j].GoldRate;   
                }
                if(map[i][j].type == ROAD) map[i][j].type = TERRAIN;
                map[i][j].difficulty[player] = map[i][j].firstDiff;
            }
        }
}

void mainWar(int player1, int player2){
    if(c[player1].soldier>c[player2].soldier){
        DestroyLoser(player2);
        c[player1].soldier -= c[player2].soldier;
        c[player2].soldier = 0;
    }
    else if(c[player1].soldier<c[player2].soldier){
        DestroyLoser(player1);
        c[player2].soldier -= c[player1].soldier;
        c[player1].soldier = 0;
    }
    else{
        DestroyLoser(player2);
        DestroyLoser(player1);
        c[player1].soldier = 0;
        c[player2].soldier = 0;
    }
}

void All_outvoidWar(int attacker, int defender){
    if(c[defender].soldier<c[attacker].soldier){
        map[c[defender].x][c[defender].y].type = TERRAIN;
        map[c[defender].x][c[defender].y].forkingdom = -1;
    }
    else{
        DestroyLoser(attacker);
        c[defender].soldier -= c[attacker].soldier;
        c[attacker].soldier = 0;
    }
}

void War(int gridX, int gridY, int player){
    int pl2 = -1;
    int Warmode;
    Warmode = checkWar(gridX, gridY, player, &pl2);
    if(Warmode == 2) All_outvoidWar(player, pl2);
    else if (Warmode == 1) mainWar(player, pl2);
    else;
}