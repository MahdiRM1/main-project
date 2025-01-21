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

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int buildRoadMode = 0; 

void drawX(int startX, int startY, int size, int thickness) {
    DrawLineEx((Vector2){startX, startY}, (Vector2){startX + size, startY + size}, thickness, BLACK);
    DrawLineEx((Vector2){startX + size, startY}, (Vector2){startX, startY + size}, thickness, BLACK);
}

void DrawPlayerInfo(int plturn) {
    Color color;
    char info[256];
    
    color = checkColor(plturn);
    
    for (int player = 0; player < kingnum; player++){
        color = checkColor(player);

        sprintf(info, "Player %d", player + 1);
        DrawText(info, y*TILE_SIZE+120, player*125 + 100, 25, color);

        if(player != plturn) color = BLACK;

        sprintf(info, "Gold: %d", c[player].gold);
        DrawText(info, y*TILE_SIZE+120, player*125 + 120, 20, color);

        sprintf(info, "Food: %d", c[player].food);
        DrawText(info, y*TILE_SIZE+120, player*125 + 135, 20, color);

        sprintf(info, "Workers: %d", c[player].worker);
        DrawText(info, y*TILE_SIZE+120, player*125 + 150, 20, color);

        sprintf(info, "Soldiers: %d", c[player].soldier);
        DrawText(info, y*TILE_SIZE+120, player*125 + 165, 20, color);

        sprintf(info, "Food rate: %d", c[player].FoodRate);
        DrawText(info, y*TILE_SIZE+120, player*125 + 180, 20, color);

        sprintf(info, "Gold rate: %d", c[player].GoldRate);
        DrawText(info, y*TILE_SIZE+120, player*125 + 195, 20, color);

        if(player > -1 && c[player].isAlive == 0) drawX(y*TILE_SIZE + 120, player*125 + 100, 110, 10);
    }
}

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
    if (map[nx][ny].forkingdom == player) sw = 1;
    }
    if (sw && map[gridX][gridY].type == TERRAIN) return 1;
    return 0;
}

void ShowTextFornSecond(double n, const char *text, int player, int ax, int ay, int size, Color color) {
    double startTime = GetTime();
    while(GetTime() - startTime < 0.2) {
        BeginDrawing();
        DrawMap(player);
        DrawText(text, ax, ay, size, color);
        EndDrawing();
    }

    while (GetTime() - startTime < n && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        BeginDrawing();
        DrawMap(player);
        DrawText(text, ax, ay, size, color);
        EndDrawing();
    }
}

void sleep(double n, int player) {
    double startTime = GetTime();
    while(GetTime() - startTime < n) {
        BeginDrawing();
        DrawMap(player);
        EndDrawing();
    }
}

void DrawMove(int player){

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
}

int update(int player) {
    int prevGold = c[player].gold;
    int prevFood = c[player].food;
    int prevWorker = c[player].worker;
    int prevSoldier = c[player].soldier;

    DrawMove(player);
    DrawPlayerInfo(player);
    DrawMap(player);

    int a = 0;
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (mouseX >= 90 && mouseX <= 210 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 0) {
                ShowTextFornSecond(1, "succesfully!",player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].gold--;
                c[player].food++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!", player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
            }
        }
        
        else if (mouseX >= 90 && mouseX <= 210 && mouseY >= 65 && mouseY <= 95) {
            if (c[player].food > 2) {
                ShowTextFornSecond(1, "succesfully!", player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].food -= 3;
                c[player].worker++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough food!", player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].worker = prevWorker;
            }
        }

        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 1) {
                ShowTextFornSecond(1, "succesfully!", player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].gold -= 2;
                c[player].soldier++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!", player, 100, x*TILE_SIZE+120, 20, RED);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].soldier = prevSoldier;
            }
        }
    
        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 65 && mouseY <= 95) {
            buildRoadMode = 1; 
            ShowTextFornSecond(20, "click to build road", player, 100, x*TILE_SIZE+120, 20, RED);
        }
        
        else if (mouseX >= 240 && mouseX <= 360 && mouseY >= 40 && mouseY <= 70){
            ShowTextFornSecond(1, "succesfully!", player, 100, x*TILE_SIZE+120, 20, RED);
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
                        if(map[gridX][gridY].forkingdom == player) for(int i=0; i<3; i++) conectionvillage(gridX, gridY, player);

                    } else map[gridX][gridY].difficulty[player] -= c[player].worker;

                    buildRoadMode = 0;
                    a = 1;
                }

                else {
                    ShowTextFornSecond(1, "Cant build road here!", player, 100, x*TILE_SIZE+120, 20, RED);
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