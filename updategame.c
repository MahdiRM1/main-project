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

int buildRoadMode = 0; 


void DrawPlayerInfo(kingdom c[], int y, int x, int plturn) {
    Color color;
    char info[256];
    
    color = checkColor(plturn);
    sprintf(info, "turn player %d", plturn+1);
    DrawText(info, 100 , x*TILE_SIZE+150, 30, color);
    
    for (int player = 0; player < kingnum; player++){
        color = checkColor(player);
    
        sprintf(info, "Gold: %d", c[player].gold);
        DrawText(info, y*TILE_SIZE+120, player*140 + 100, 25, color);

        sprintf(info, "Food: %d", c[player].food);
        DrawText(info, y*TILE_SIZE+120, player*140 + 120, 25, color);

        sprintf(info, "Workers: %d", c[player].worker);
        DrawText(info, y*TILE_SIZE+120, player*140 + 140, 25, color);

        sprintf(info, "Soldiers: %d", c[player].soldier);
        DrawText(info, y*TILE_SIZE+120, player*140 + 160, 25, color);

        sprintf(info, "Food generation rate: %d", c[player].FoodRate);
        DrawText(info, y*TILE_SIZE+120, player*140 + 180, 25, color);

        sprintf(info, "Gold generation rate: %d", c[player].GoldRate);
        DrawText(info, y*TILE_SIZE+120, player*140 + 200, 25, color);
    }
}

void conectionvillage(int gridX, int gridY, int player) { 
    int sw = 0;
    if (gridX < MAP_SIZE && map[gridX + 1][gridY].type == VILLAGE && map[gridX + 1][gridY].forkingdom != player){ gridX++; sw = 1;}
    else if (gridY < MAP_SIZE && map[gridX][gridY + 1].type == VILLAGE  && map[gridX][gridY + 1].forkingdom != player){ gridY++; sw = 1;}
    else if (gridY > 0 && map[gridX][gridY - 1].type == VILLAGE  && map[gridX][gridY - 1].forkingdom != player){ gridY--; sw = 1;}
    else if (gridX > 0 && map[gridX - 1][gridY].type == VILLAGE  && map[gridX - 1][gridY].forkingdom != player){ gridX--; sw = 1;}
    if(sw && map[gridX][gridY].forkingdom != player){
        map[gridX][gridY].forkingdom = player;
        c[player].FoodRate += map[gridX][gridY].FoodRate;
        c[player].GoldRate += map[gridX][gridY].GoldRate;
    }
}


int canBuild(int x, int y, int player) {    
    if ((x > 0 && map[x - 1][y].forkingdom == player)  ||
        (x < MAP_SIZE && map[x + 1][y].forkingdom == player ) ||
        (y < MAP_SIZE && map[x][y + 1].forkingdom == player ) ||
        (y > 0 &&  map[x][y - 1].forkingdom == player) ){
        if (map[x][y].type == TERRAIN) return 1;
    }
    return 0;
}

void ShowTextFornSecond(double n, const char *text, int player, Color color) 
{
    double startTime = GetTime();
    while(GetTime() - startTime < 0.2) {
        BeginDrawing();
        DrawMap(player);
        DrawText(text, 100, x*TILE_SIZE+120, 20, color);
        EndDrawing();
    }
    while (GetTime() - startTime < n && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        BeginDrawing();
        DrawMap(player);
        DrawText(text, 100, x*TILE_SIZE+120, 20, color);
        EndDrawing();
    }
}

int update(int player) {
    int prevGold = c[player].gold;
    int prevFood = c[player].food;
    int prevWorker = c[player].worker;
    int prevSoldier = c[player].soldier;

    Color color;
    color = checkColor(player);
    DrawRectangle(90, 15, 120, 30, color);
    DrawRectangleLines(90, 15, 120, 30, BLACK);
    DrawText("buy food", 100, 20, 20, BLACK);
    DrawRectangle(90, 65, 130, 30, color);
    DrawRectangleLines(90, 65, 130, 30, BLACK);
    DrawText("buy worker", 100, 70, 20, BLACK);
    DrawRectangle(390, 15, 125, 30, color);
    DrawRectangleLines(390, 15, 125, 30, BLACK);
    DrawText("buy soldier", 400, 20, 20, BLACK);
    DrawRectangle(390, 65, 120, 30, color);
    DrawRectangleLines(390, 65, 120, 30, BLACK);
    DrawText("build road", 400, 70, 20, BLACK);
    DrawRectangle(240, 40, 120, 30, color);
    DrawRectangleLines(240, 40, 120, 30, BLACK);
    DrawText("do nothing", 250, 45, 20, BLACK);
    DrawPlayerInfo(c, y, x, player);
    DrawMap(player);

    int a = 0;
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (mouseX >= 90 && mouseX <= 210 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 0) {
                ShowTextFornSecond(1, "succesfully!",player, RED);
                c[player].gold--;
                c[player].food++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!",player, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
            }
        }
        else if (mouseX >= 90 && mouseX <= 210 && mouseY >= 65 && mouseY <= 95) {
            if (c[player].food > 2) {
                ShowTextFornSecond(1, "succesfully!",player, RED);
                c[player].food -= 3;
                c[player].worker++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough food!",player, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].worker = prevWorker;
            }
        }
        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 1) {
                ShowTextFornSecond(1, "succesfully!",player, RED);
                c[player].gold -= 2;
                c[player].soldier++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!",player, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].soldier = prevSoldier;
            }
        }
    
        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 65 && mouseY <= 95) {
            buildRoadMode = 1; 
            ShowTextFornSecond(20, "click to build road",player, RED);
        }
        
        else if (mouseX >= 240 && mouseX <= 360 && mouseY >= 40 && mouseY <= 70){
            ShowTextFornSecond(1, "succesfully!",player, RED);
            a = 1;
        }

        
        if (buildRoadMode) {
            Vector2 mousePosition = GetMousePosition();
            int gridX = (mousePosition.x / TILE_SIZE) - 2;
            int gridY = (mousePosition.y / TILE_SIZE) - 2;
            if (gridX >= 0 && gridX < x && gridY >= 0 && gridY < y) {
                if (canBuild(gridX, gridY, player)) {
                    if (map[gridX][gridY].difficulty[player] <= c[player].worker) {
                        map[gridX][gridY].type = ROAD;
                        map[gridX][gridY].forkingdom = player;
                        War(gridX, gridY, player);
                        if(map[gridX][gridY].forkingdom == player) conectionvillage(gridX, gridY, player);
                        if(map[gridX][gridY].forkingdom == player) conectionvillage(gridX, gridY, player);
                        if(map[gridX][gridY].forkingdom == player) conectionvillage(gridX, gridY, player);
                    } else {
                        map[gridX][gridY].difficulty[player] -= c[player].worker;
                    }
                    buildRoadMode = 0;
                    a = 1;
                }
                else {
                    ShowTextFornSecond(1, "Cant build road here!",player, RED);
                }
            }
        }
    }

    if (a) {
        c[player].gold += c[player].GoldRate;
        c[player].food += c[player].FoodRate;
    }

    return a;
}
