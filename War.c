#include <stdio.h>
#include "updategame.h"
#include "generateMap.h"
#include "War.h"
#include "printmap.h"
#include "info.h"

extern kingdom c[4];
extern Tile map[17][17];
extern int x, y;
extern int alivePlayers;
extern int dx[4];
extern int dy[4];

int checkWar(fighter fighters[]) {
    int counter = 1;
    for (int i = 0; i < 4; i++) {
        int nx = fighters[0].x + dx[i];
        int ny = fighters[0].y + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom != fighters[0].player && map[nx][ny].forkingdom != -1) {
            int sw = 1;
            for(int j = 1; j < counter; j++){
                if(fighters[j].player == map[nx][ny].forkingdom){
                    if(map[nx][ny].type == KINGDOM){
                        sw = 0;
                        fighters[j].x = nx;
                        fighters[j].y = ny;
                        fighters[j].player = map[nx][ny].forkingdom;
                    }
                }
            }
            if(sw){
                fighters[counter].x = nx;
                fighters[counter].y = ny;
                fighters[counter].player = map[nx][ny].forkingdom;
                counter++;
            }
        }
    }
    return counter;
}


void Destroy(int nx, int ny){
    extern Sound bubble;
    int player = map[nx][ny].forkingdom;
    PlaySound(bubble);
    sleep(0.1, player);
    if(map[nx][ny].type == ROAD)map[nx][ny].type = TERRAIN;
    else if(map[nx][ny].type == VILLAGE){
        c[player].FoodRate -= map[nx][ny].FoodRate;
        c[player].GoldRate -= map[nx][ny].GoldRate;
    }
    map[nx][ny].difficulty[player] = map[nx][ny].firstDiff;
    map[nx][ny].forkingdom = -1;
}


void lowDestroy(int gridX, int gridY){
    int player = map[gridX][gridY].forkingdom;
    if(map[gridX][gridY].type != ROAD) return;
    int sw = 0;
    int destroyx, destroyy;
    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == player){
            sw++;
            destroyx = nx; destroyy = ny;
        }
    }
    switch (sw){
        case 4:
        case 3:
        case 2: return;
        case 1:{
            Destroy(gridX, gridY);
            lowDestroy(destroyx, destroyy);
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


int checkKingdom(int checkX, int checkY, int isCheck[][17]) {
    int player = map[checkX][checkY].forkingdom;

    for (int i = 0; i < 4; i++) {
        int nx = checkX + dx[i];
        int ny = checkY + dy[i];
        int count = 0;
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE &&
            map[nx][ny].forkingdom == player && !isCheck[nx][ny]) {
                count++;
                isCheck[nx][ny] = 1;

            if (map[nx][ny].type == KINGDOM) return 1;
            else if(!count) return 0;
            
            if (checkKingdom(nx, ny, isCheck)){
                return 1;
            }
        }
    }

    return 0;
}


void DestroyLoser(fighter loser){
    Destroy(loser.x, loser.y);

    for (int i = 0; i < 4; i++) {
        int nx = loser.x + dx[i];
        int ny = loser.y + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == loser.player && map[nx][ny].type != KINGDOM) {
            int isCheck[17][17] = {0};
            int checkking = checkKingdom(nx, ny, isCheck);
            if(checkking) {
                if(map[loser.x][loser.y].type == VILLAGE){
                    loser.x = nx;
                    loser.y = ny;
                    DestroyLoser(loser);
                }
                else lowDestroy(nx, ny);
            }
            else highDestroy(nx, ny);
        }
    }
}


void DestroyAll(int player){
    
    map[c[player].x][c[player].y].forkingdom = -1;
    map[c[player].x][c[player].y].type = BLOCK_HOUSE;
    alivePlayers--;
    c[player].isAlive = 0;
    for (int i = 0; i < x; i++) 
        for (int j = 0; j < y; j++) {
            if(map[i][j].forkingdom == player && map[i][j].type != KINGDOM) {
                Destroy(i, j);
            }
        }
}


void mainWar(fighter fighter1, fighter fighter2){

    if(c[fighter1.player].soldier>c[fighter2.player].soldier){
        DestroyLoser(fighter2);
        c[fighter1.player].soldier -= c[fighter2.player].soldier;
        c[fighter2.player].soldier = 0;
        c[fighter2.player].price_soldier = 1;
    }
    else if(c[fighter1.player].soldier<c[fighter2.player].soldier){
        DestroyLoser(fighter1);
        c[fighter2.player].soldier -= c[fighter1.player].soldier;
        c[fighter1.player].soldier = 0;
        c[fighter1.player].price_soldier = 1;
    }
    else{
        DestroyLoser(fighter1);
        DestroyLoser(fighter2);
        c[fighter1.player].soldier = 0;
        c[fighter1.player].price_soldier = 1;
        c[fighter2.player].soldier = 0;
        c[fighter2.player].price_soldier = 1;
    }
}


void AllOutWar(fighter attacker, int defender) {

    extern Sound lose; 

    if (c[defender].soldier < c[attacker.player].soldier) {
        PlaySound(lose);

        char info[100];
        sprintf(info, "Player %d loooooose", defender + 1);
        Color color = checkColor(defender);
        ShowTextFornSecond(1, info, defender, 50, 50, 50, color);

        DestroyAll(defender);
    } else {

        DestroyLoser(attacker);
        c[defender].soldier -= c[attacker.player].soldier;
        c[attacker.player].soldier = 0;
        c[attacker.player].price_soldier = 1;
    }
}



void nFightersWar(fighter fighters[], int n){   
    extern Sound fight;
    if(n == 1) return;
    PlaySound(fight);
    ShowTextFornSecond(0.5, "War!", fighters[0].player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, WHITE);
    for(int i = 1; i < n; i++)
        if(map[fighters[0].x][fighters[0].y].forkingdom == fighters[0].player){
            if(map[fighters[i].x][fighters[i].y].type == KINGDOM){
                AllOutWar(fighters[0], fighters[i].player);
            }
            else if(map[fighters[i].x][fighters[i].y].forkingdom == fighters[i].player) mainWar(fighters[0], fighters[i]);
    }
}


void swapFighters(fighter fighters[], int i, int j){
    fighter tmp;
    tmp = fighters[i];
    fighters[i] = fighters[j];
    fighters[j] = tmp;
}


void sortFighters(fighter fighters[], int n){
    int countnokingdom = 0;
    for(int i = 1; i < n; i++) if(map[fighters[i].x][fighters[i].y].type != KINGDOM) swapFighters(fighters, i, ++countnokingdom);
    for(int i = 1; i <= countnokingdom; i++)
        for(int j = i + 1; j <= countnokingdom; j++)
            if(c[fighters[i].player].soldier > c[fighters[j].player].soldier) swapFighters(fighters, i, j);
    
    for(int i = countnokingdom + 1; i < n; i++)
        for(int j = i + 1; j < n; j++)
            if(c[fighters[i].player].soldier > c[fighters[j].player].soldier) swapFighters(fighters, i, j);
}


void War(int gridX, int gridY, int player){
    fighter fighters[4] = {{gridX, gridY, player}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
    int Warmode = checkWar(fighters);
    sortFighters(fighters, Warmode);
    nFightersWar(fighters, Warmode);
}