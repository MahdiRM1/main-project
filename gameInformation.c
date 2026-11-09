#include <stdio.h>
#include <stdlib.h>
#include "generateMap.h"
#include "gameInformation.h"
#include "raylib.h"

extern Tile map[17][17];
extern kingdom c[4];
extern int x, y;
extern int kingnum;
extern int alivePlayers;
extern int minutes, seconds;
int agominutes = 0, agoseconds = 0;

void GetInformation(){
    int n, i;
    while(true){
        printf("enter x and y for map:"); scanf("%d %d", &x, &y);
        if(x <= MAP_SIZE && x>0 && y <= MAP_SIZE && y>0) break;
        else printf("x and y should be less than 17\n");
    }
    generate_array();
    while (true){
        printf("enter number of kingdoms:");
        scanf("%d", &kingnum);
        alivePlayers = kingnum;
        if(kingnum < 5 && kingnum > 0) break;
        else printf("number of kingdoms should be between 1 and 4\n");
    }
    for(i=0; i<kingnum; i++){
        printf("enter location of kingdom %d:", i+1); scanf("%d %d", &c[i].x, &c[i].y);
        printf("If you want this player to be the computer, enter the number 1\notherwise enter the number 0.\n");  scanf("%d", &c[i].is_computer);
        c[i].x--; c[i].y--; c[i].GoldRate=1; c[i].FoodRate=0; c[i].gold=5; c[i].food=0; c[i].worker=1;  c[i].price_soldier = 1;
        c[i].soldier=0; map[c[i].x][c[i].y].forkingdom = i; c[i].isAlive = 1; map[c[i].x][c[i].y].type = KINGDOM;  c[i].price_worker = 3;
    } 

    int v;
    while(true){
        printf("enter number of villages:");
        scanf("%d", &v);
        if(v <= 10) break;
        else printf("number of villages should be less than 10\n");
    } 
    for(i=0; i<v; i++) {
        int vx, vy;
        int foodrate, goldrate;
        printf("enter location of village %d:", i+1);
        scanf("%d %d", &vx, &vy);
        vx--; vy--;
        printf("enter food generation rate and gold generation rate for village %d:", i+1);
        scanf("%d %d", &foodrate, &goldrate);
        map[vx][vy].type = VILLAGE; map[vx][vy].FoodRate = foodrate; map[vx][vy].GoldRate = goldrate;
    }

    printf("enter number of blocked houses:");
    scanf("%d", &n);
    for(i=0; i<n; i++) {
        int pi, qi;
        printf("enter location of blocked house %d:", i+1);
        scanf("%d %d", &pi, &qi);
        map[pi-1][qi-1].type = BLOCK_HOUSE;
    }
}


int GetasFile(){
    int i, n;
    FILE *file;

    file = fopen("map.txt","rt");
    if(!file){
        printf("cant open file\n");
        return -1;
    }

    fscanf(file, "%d %d", &x, &y);
    generate_array();
    fscanf(file, "%d", &kingnum);
    alivePlayers = kingnum; 

    for(i=0; i<kingnum; i++){
        fscanf(file, "%d %d", &c[i].x, &c[i].y);
        fscanf(file, "%d", &c[i].is_computer);
        c[i].x--; c[i].y--; c[i].GoldRate=1; c[i].FoodRate=0; c[i].gold=5; c[i].food=0; c[i].worker=1; c[i].price_worker = 2;
        c[i].soldier=0; map[c[i].x][c[i].y].forkingdom = i; c[i].isAlive = 1; map[c[i].x][c[i].y].type = KINGDOM; c[i].price_soldier = 1;
    }

    int v;
    fscanf(file, "%d", &v);

    for(i=0; i<v; i++) {
        int vx, vy;
        int foodrate, goldrate;
        fscanf(file, "%d %d", &vx, &vy);
        vx--; vy--;
        fscanf(file, "%d %d", &foodrate, &goldrate);
        map[vx][vy].type = VILLAGE; map[vx][vy].FoodRate = foodrate; map[vx][vy].GoldRate = goldrate;
    }

    fscanf(file, "%d", &n);
    for(i=0; i<n; i++) {
        int pi, qi;
        fscanf(file, "%d %d", &pi, &qi);
        map[pi-1][qi-1].type = BLOCK_HOUSE;
    }

    fclose(file);
    return 0;
}


Game gamestruct(){
    Game game;
    for(int i=0; i<17; i++)
        for(int j=0; j<17; j++)
            game.map[i][j] = map[i][j];
    for(int i=0; i<4; i++)
        game.c[i] = c[i];
    game.aliveplayers = alivePlayers;
    game.kingnum = kingnum;
    game.x = x;
    game.y = y; 
    game.minutes = minutes;
    game.seconds = seconds;
    return game;
}


void savegame(int plturn){
    Game game = gamestruct();
    FILE *file;
    file = fopen("save.dat", "wb");
    fwrite(&game, sizeof(Game), 1, file);
    fwrite(&plturn, sizeof(int), 1, file);
    fclose(file);
}

int loadgame(){
    Game game;
    FILE *file;
    file = fopen("save.dat", "rb");
    if(!file){
        printf("cant load game\n");
        return -1;
    }
    fread(&game, sizeof(Game), 1, file);
    if(game.aliveplayers < 2){
        printf("cant load game\n");
        fclose(file);
        return -1;
    }
    for(int i=0; i<17; i++)
        for(int j=0; j<17; j++)
            map[i][j] = game.map[i][j];
    for(int i=0; i<4; i++)
        c[i] = game.c[i];
    alivePlayers = game.aliveplayers;
    kingnum = game.kingnum;
    x = game.x;
    y = game.y;
    agominutes = game.minutes;
    agoseconds = game.seconds;
    int plturn;
    fread(&plturn, sizeof(int), 1, file);
    fclose(file);
    return plturn;
}
