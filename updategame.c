#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "updategame.h"
#include "generateMap.h"
#include "printmap.h"
#include "raylib.h"
#include "War.h"  
#include "info.h"
#include "Computer.h"

extern kingdom c[4];
extern int kingnum;
extern Tile map[17][17];
extern int x, y;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int buildRoadMode = 0; 

void conectionvillage(int gridX, int gridY, int player) { 
    for (int i = 0; i < 4; i++) {
        int sw = 0;
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];

        if(nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE &&
        map[nx][ny].type == VILLAGE && map[nx][ny].forkingdom != player) sw = 1;
        if(sw && map[nx][ny].forkingdom != player){
            map[nx][ny].forkingdom = player;
            c[player].FoodRate += map[nx][ny].FoodRate;
            c[player].GoldRate += map[nx][ny].GoldRate;
        }
    }
}


int canBuild(int gridX, int gridY, int player) {    
    int sw = 0;
    for (int i = 0; i < 4; i++) {
        int nx = gridX + dx[i];
        int ny = gridY + dy[i];
    if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[nx][ny].forkingdom == player) sw = 1;
    }
    if (sw && map[gridX][gridY].type == TERRAIN) return 1;
    return 0;
}


void random(int plturn) {
    extern Sound badLuck;
    extern Sound ding;
    char info[100];
    Color color;
    // 30% chance to pick a number between 1 and 4
    if ((rand() % 100) < 30) {
        int player = (rand() % kingnum);
        // Pick a random number between 1 and 2
        int choice = (rand() % 2) + 1;

        // Return a random number between -2 and 2
        if(choice == 1){ // Food
            int count = (rand() % 5) - 2;
            if(c[player].food >= count) c[player].food -= count;
            else c[player].food = 0;
            
            if(count<0) {
                PlaySound(ding);
                sprintf(info, "%d food items added to player %d", -count, player+1);
            }
            else if(count>0){
                PlaySound(badLuck);
                sprintf(info, "%d food items decided as player %d", count, player+1);
            }
            else return;
            if(!c[player].isAlive) return;
            ShowTextFornSecond(1, info, plturn, 50, 50, 20, checkColor(player));
        }
        else if (choice == 2){ //Gold
            int count = (rand() % 5) - 2;
            if (c[player].gold >= count) c[player].gold -= count;
            else c[player].gold = 0;

            if(count<0){
                PlaySound(ding);
                sprintf(info, "%d gold items added to player %d", -count, player+1);
            }
            else if(count>0){
                PlaySound(badLuck);
                sprintf(info, "%d gold items decided as player %d", count, player+1);
            }
            else return;
            if(!c[player].isAlive) return;
            ShowTextFornSecond(1, info, plturn, 50, 50, 20, checkColor(player));
        }
    }
}

int update(int player) {
    extern Sound wrong;

    srand(time(0)); // For random generate food and gold

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = (screenWidth - (x * TILE_SIZE)) / 2;
    int centerY = (screenHeight - (y * TILE_SIZE)) / 2;

    DrawMap(player);
    DrawMove(player);
    DrawPlayerInfo(player);

    int a = 0;
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int startY = 200;

    if(c[player].is_computer == 1) {
        a = computer(player);
        if (a) {
        c[player].gold += c[player].GoldRate;
        c[player].food += c[player].FoodRate;
        random(player);
    }
    return a ;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        extern Sound click;
        if (mouseX >= 20 && mouseX <= 220 && mouseY >= startY && mouseY <= startY + 50) {
            // Buy food
            if (c[player].gold > 0) {
                PlaySound(click);
                ShowTextFornSecond(1, "successfully!", player, 100, 100, 20, RED);
                c[player].gold--;
                c[player].food++;
                a = 1;
            } else {
                PlaySound(wrong);
                ShowTextFornSecond(1, "Not enough gold!", player, 100, 100, 20, RED);
            }
        } else if (mouseX >= 20 && mouseX <= 220 && mouseY >= startY + 100 && mouseY <= startY + 150) {
            // Buy worker
            if (c[player].food >= c[player].price_worker) {
                PlaySound(click);
                ShowTextFornSecond(1, "successfully!", player, 100, 100, 20, RED);
                c[player].food -= c[player].price_worker;
                c[player].worker++;
                c[player].price_worker++;
                a = 1;
            } else {
                PlaySound(wrong);
                ShowTextFornSecond(1, "Not enough food!", player, 100, 100, 20, RED);
            }
        } else if (mouseX >= 20 && mouseX <= 220 && mouseY >= startY + 200 && mouseY <= startY + 250) {
            // Buy soldier
            if (c[player].gold >=c[player].price_soldier) {
                PlaySound(click);
                ShowTextFornSecond(1, "successfully!", player, 100, 100, 20, RED);
                c[player].gold -= c[player].price_soldier;
                c[player].soldier++;
                c[player].price_soldier++;
                a = 1;
            } else {
                PlaySound(wrong);
                ShowTextFornSecond(1, "Not enough gold!", player, 100, 100, 20, RED);
            }
        } else if (mouseX >= 20 && mouseX <= 220 && mouseY >= startY + 300 && mouseY <= startY + 350) {
            // Do nothing
            PlaySound(click);
            ShowTextFornSecond(1, "successfully!", player, 100, 100, 20, RED);
            a = 1;
        }

        else {
            int gridX = (mouseX - centerX) / TILE_SIZE;
            int gridY = (mouseY - centerY) / TILE_SIZE;

            if (gridX >= 0 && gridX < x && gridY >= 0 && gridY < y) {
                if (canBuild(gridX, gridY, player)) {
                    PlaySound(click);
                    if (map[gridX][gridY].difficulty[player] <= c[player].worker) {
                        map[gridX][gridY].type = ROAD;
                        map[gridX][gridY].forkingdom = player;
                        War(gridX, gridY, player);
                        if(map[gridX][gridY].forkingdom == player)
                            for(int i=0; i<3; i++)
                                conectionvillage(gridX, gridY, player);
                    } else {
                        map[gridX][gridY].difficulty[player] -= c[player].worker;
                    }
                    a = 1;
                } else {
                    PlaySound(wrong);
                    ShowTextFornSecond(1, "Can't build road here!", player, 100, 100, 20, RED);
                }
            }
        }
    }

    if (a) {
        c[player].gold += c[player].GoldRate;
        c[player].food += c[player].FoodRate;
        random(player);
    }

    return a;
}