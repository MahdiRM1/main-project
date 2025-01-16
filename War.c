#include <stdio.h>
#include "updategame.h"
#include "generateArray.h"
#include "War.h"

extern kingdom c[4];
extern Tile map[17][17];
extern int x, y;
extern int alivePlayers;
extern int dx[4];
extern int dy[4];

int checkWar(int gridX, int gridY, int player, int *x, int *y) {

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].type == KINGDOM && map[nx][ny].forkingdom != player) {
            *x = nx;
            *y = ny;
            return 2;
        }
    }

    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE &&
            map[nx][ny].forkingdom != player && map[nx][ny].forkingdom != -1) {
            
            *x = nx;
            *y = ny;

            return 1;
        }
    }
    return 0;
}

void Destroy(int x, int y){
    int player = map[x][y].forkingdom;
    if(map[x][y].type == ROAD)map[x][y].type = TERRAIN;
    map[x][y].difficulty[player] = map[x][y].firstDiff;
    map[x][y].forkingdom = -1;
}


void lowDestroy(int gridX, int gridY){
    int player = map[gridX][gridY].forkingdom;
    if(map[gridX][gridY].type != ROAD) return;
    int sw = 0;
    int x, y;
    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == player){
            sw++;
            x = nx; y = ny;
        }
    }
    switch (sw){
        case 4:
        case 3:
        case 2: return;
        case 1:{
            Destroy(gridX, gridY);
            lowDestroy(x, y);
            break;
        }
        case 0:{
            Destroy(gridX, gridY);
            break;
        }
        default: break;
    }
}



void highDestroy(int gridX, int gridY){
    int player = map[gridX][gridY].forkingdom;
    if(map[gridX][gridY].type == KINGDOM) return;
    Destroy(gridX, gridY);
    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == player) {
            highDestroy(nx, ny);
        }
    }
}


int checkKingdom(int x, int y, int lose) {
    int visited[MAP_SIZE][MAP_SIZE] = {0};
    int player = map[x][y].forkingdom;

    for (int i = 0; i < 4; i++) {
        int adjX = x + dx[i];
        int adjY = y + dy[i];
        int count = 0;
        if((dx[lose]+dx[i])==0 && (dy[lose]+dy[i])==0) continue;

        if (adjX >= 0 && adjX < MAP_SIZE && adjY >= 0 && adjY < MAP_SIZE &&
            map[adjX][adjY].forkingdom == player && !visited[adjX][adjY]) {
            count++;

            if (map[adjX][adjY].type == KINGDOM) return 1;
            else if(!count) return 0;
            
            visited[adjX][adjY] = 1;

            if (checkKingdom(adjX, adjY, i)) return 1;
        }
    }

    return 0;
}



void DestroyLoser(int x, int y){
    int player = map[x][y].forkingdom;
    ShowTextFornSecond(0.5, "War", player, x*TILE_SIZE + 100, y*TILE_SIZE + 100, 10, BLACK);
    Destroy(x, y);
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == player && map[nx][ny].type == ROAD) {
            if(checkKingdom(nx, ny, i)) lowDestroy(nx, ny);
            else highDestroy(nx, ny);
        }
    }
}

void DestroyAll(int player){
    int i, j;
    ShowTextFornSecond(0.5, "War", player, c[player].x*TILE_SIZE + 100, c[player].y*TILE_SIZE + 100, 10, BLACK);
    map[c[player].x][c[player].y].forkingdom = -1;
    map[c[player].x][c[player].y].type = BLOCK_HOUSE;
    alivePlayers--;
    c[player].isAlive = 0;
    for (i = 0; i < x; i++) 
        for (j = 0; j < y; j++) {
            if(map[i][j].forkingdom == player && map[i][j].type != KINGDOM) {
                Destroy(i, j);
            }
        }
}

void mainWar(int x1, int y1, int x2, int y2){

    if(c[map[x1][y1].forkingdom].soldier>c[map[x2][y2].forkingdom].soldier){
        DestroyLoser(x2, y2);
        c[map[x1][y1].forkingdom].soldier -= c[map[x2][y2].forkingdom].soldier;
        c[map[x2][y2].forkingdom].soldier = 0;
    }
    else if(c[map[x1][y1].forkingdom].soldier<c[map[x2][y2].forkingdom].soldier){
        DestroyLoser(x1, y1);
        c[map[x2][y2].forkingdom].soldier -= c[map[x1][y1].forkingdom].soldier;
        c[map[x1][y1].forkingdom].soldier = 0;
    }
    else{
        DestroyLoser(x1, y1);
        DestroyLoser(x2, y2);
        c[map[x1][y1].forkingdom].soldier = 0;
        c[map[x2][y2].forkingdom].soldier = 0;
    }
}

void All_outvoidWar(int x1, int y1, int defender){
    if(c[defender].soldier<c[map[x1][y1].forkingdom].soldier) DestroyAll(defender);
    else{
        DestroyLoser(x1, y1);
        c[defender].soldier -= c[map[x1][y1].forkingdom].soldier;
        c[map[x1][y1].forkingdom].soldier = 0;
    }
}

void War(int gridX, int gridY, int player){
    int x, y;
    int Warmode;
    Warmode = checkWar(gridX, gridY, player, &x, &y);
    if(Warmode == 2) All_outvoidWar(gridX, gridY, map[x][y].forkingdom);
    else if (Warmode == 1) mainWar(gridX, gridY, x, y);
    else;
}