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

int checkWar(fighter fighters[]) {
    int counter = 1;
    for (int i = 0; i < 4; i++) {
        int nx = fighters[0].x + dx[i];
        int ny = fighters[0].y + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom != fighters[0].player && map[nx][ny].forkingdom != -1) {
            int sw = 1;
            for(int j = 1; j < counter; j++){
                if(fighters[j].player == map[nx][ny].forkingdom){
                    sw = 0;
                    if(map[nx][ny].type == KINGDOM){
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
    int player = map[nx][ny].forkingdom;
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


int checkKingdom(int checkX, int checkY, int Previous) {
    int player = map[checkX][checkY].forkingdom;

    for (int i = 0; i < 4; i++) {
        int nx = checkX + dx[i];
        int ny = checkY + dy[i];
        int count = 0;
        if((dx[Previous]+dx[i])==0 && (dy[Previous]+dy[i])==0) continue;

        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE &&
            map[nx][ny].forkingdom == player) {
            count++;

            if (map[nx][ny].type == KINGDOM) return 1;
            else if(!count) return 0;
            
            if (checkKingdom(nx, ny, i)) return 1;
        }
    }

    return 0;
}


void DestroyLoser(fighter loser){
    Destroy(loser.x, loser.y);
    for (int i = 0; i < 4; i++) {
        int nx = loser.x + dx[i];
        int ny = loser.y + dy[i];
        if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == loser.player) {
            if(checkKingdom(nx, ny, i)) {
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
        ShowTextFornSecond(0.5, "War!", fighter2.player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, BLACK);
        DestroyLoser(fighter2);
        c[fighter1.player].soldier -= c[fighter2.player].soldier;
        c[fighter2.player].soldier = 0;
    }
    else if(c[fighter1.player].soldier<c[fighter2.player].soldier){
        ShowTextFornSecond(0.5, "War!", fighter1.player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, BLACK);
        DestroyLoser(fighter1);
        c[fighter2.player].soldier -= c[fighter1.player].soldier;
        c[fighter1.player].soldier = 0;
    }
    else{
        ShowTextFornSecond(0.5, "War!", fighter1.player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, BLACK);
        DestroyLoser(fighter1);
        DestroyLoser(fighter2);
        c[fighter1.player].soldier = 0;
        c[fighter2.player].soldier = 0;
    }
}


void All_outvoidWar(fighter attacker, int defender){
    if(c[defender].soldier<c[attacker.player].soldier){
        ShowTextFornSecond(0.5, "Big War!", attacker.player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, BLACK);
        DestroyAll(defender);
    }
    else{
        ShowTextFornSecond(0.5, "War!", attacker.player, x/2*TILE_SIZE + 100, y/2*TILE_SIZE + 100, 100, BLACK);
        DestroyLoser(attacker);
        c[defender].soldier -= c[attacker.player].soldier;
        c[attacker.player].soldier = 0;
    }
}


void TreeManWar(fighter fighters[]){

    //At least one all out void war
    if(map[fighters[1].x][fighters[1].y].type == KINGDOM){
        All_outvoidWar(fighters[0], fighters[1].player);
        if(map[fighters[0].x][fighters[0].y].forkingdom == fighters[0].player) {
            if(map[fighters[2].x][fighters[2].y].type == KINGDOM) All_outvoidWar(fighters[0], fighters[2].player);
            else mainWar(fighters[0], fighters[2]);
        }
        return;
    }
    
    else if(map[fighters[2].x][fighters[2].y].type == KINGDOM){
        All_outvoidWar(fighters[0], fighters[2].player);
        if(map[fighters[0].x][fighters[0].y].forkingdom == fighters[0].player){
            if(map[fighters[1].x][fighters[1].y].type == KINGDOM) All_outvoidWar(fighters[0], fighters[1].player);
            else mainWar(fighters[0], fighters[1]);
        }
        return;
    }

    //two main wars
    if(c[fighters[1].player].soldier < c[fighters[2].player].soldier){
        mainWar(fighters[0], fighters[1]);
        if(map[fighters[0].x][fighters[0].y].forkingdom == fighters[0].player) mainWar(fighters[0], fighters[2]);
        return;
    }
    else if(c[fighters[1].player].soldier > c[fighters[2].player].soldier){
        mainWar(fighters[0], fighters[2]);
        if(map[fighters[0].x][fighters[0].y].forkingdom == fighters[0].player) mainWar(fighters[0], fighters[1]);
        return;
    }
    else {
        if(c[fighters[0].player].soldier > c[fighters[1].player].soldier + c[fighters[2].player].soldier){
            DestroyLoser(fighters[1]);
            DestroyLoser(fighters[2]);
            c[fighters[0].player].soldier -= (c[fighters[1].player].soldier + c[fighters[1].player].soldier);
            c[fighters[1].player].soldier = 0;
            c[fighters[2].player].soldier = 0;
            return;
        }
        else if(c[fighters[0].player].soldier < c[fighters[1].player].soldier + c[fighters[2].player].soldier){
            DestroyLoser(fighters[0]);
            c[fighters[1].player].soldier -= (c[fighters[0].player].soldier / 2);
            c[fighters[2].player].soldier -= (c[fighters[0].player].soldier / 2);
            c[fighters[0].player].soldier = 0;
            return;
        }
        else {
            DestroyLoser(fighters[0]);
            DestroyLoser(fighters[1]);
            DestroyLoser(fighters[2]);
            c[fighters[0].player].soldier = 0;
            c[fighters[1].player].soldier = 0;
            c[fighters[2].player].soldier = 0;
            return;
        }
    }
}


void War(int gridX, int gridY, int player){
    fighter fighters[4] = {{gridX, gridY, player}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
    int Warmode = checkWar(fighters);
    switch (Warmode){
        case 1: break;
        case 2:{
            if(map[fighters[1].x][fighters[1].y].type == KINGDOM) All_outvoidWar(fighters[0], fighters[1].player);
            else mainWar(fighters[0], fighters[1]);
            break;
        }
        case 3:{
            TreeManWar(fighters);
            break;
        }
    }
}