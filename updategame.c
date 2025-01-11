#include <stdio.h>
#include <unistd.h>
#include "updategame.h"
#include "generateArray.h"
#include "printmap.h"
#include "raylib.h"

extern kingdom c[4];
extern int kingnum;
extern Tile map[17][17];
extern int v;
extern village Vill[10];

int buildRoadMode = 0; 

void DrawPlayerInfo(kingdom c[], int y, int x, int plturn) {
    Color color;
    char info[256];
    switch (plturn) {
            case 0: color = BLUE; break;
            case 1: color = RED; break;
            case 2: color = ORANGE; break;
            case 3: color = YELLOW; break;
        }
    sprintf(info, "turn player %d", plturn+1);
    DrawText(info, 400, x*TILE_SIZE+120, 25, color);
    for (int player = 0; player < kingnum; player++){
        switch (player) {
            case 0: color = BLUE; break;
            case 1: color = RED; break;
            case 2: color = ORANGE; break;
            case 3: color = YELLOW; break;
        }
    
        sprintf(info, "Gold: %d", c[player].gold);
        DrawText(info, y*TILE_SIZE+120, player*140 + 100, 25, color);

        sprintf(info, "Food: %d", c[player].food);
        DrawText(info, y*TILE_SIZE+120, player*140 + 120, 25, color);

        sprintf(info, "Workers: %d", c[player].worker);
        DrawText(info, y*TILE_SIZE+120, player*140 + 140, 25, color);

        sprintf(info, "Soldiers: %d", c[player].soldier);
        DrawText(info, y*TILE_SIZE+120, player*140 + 160, 25, color);

        sprintf(info, "Food generation rate: %d", c[player].makeFood);
        DrawText(info, y*TILE_SIZE+120, player*140 + 180, 25, color);

        sprintf(info, "Gold generation rate: %d", c[player].makeGold);
        DrawText(info, y*TILE_SIZE+120, player*140 + 200, 25, color);
    }
}

int conectionvillage(int villx, int villy) { 
    if ((map[(villx) + 1][villy].type == ROAD) || 
        (map[villx][(villy) + 1].type == ROAD) || 
        (map[villx][(villy) - 1].type == ROAD) || 
        (map[(villx) - 1][villy].type == ROAD)) {
        return 1;
    }
    return 0;
}

int canBuild(int x, int y) {
    if (map[x - 1][y].type == ROAD  ||
        map[x + 1][y].type == ROAD  ||
        map[x][y + 1].type == ROAD  ||
        map[x][y - 1].type == ROAD  ||
        map[x - 1][y].type == KINGDOM || 
        map[x + 1][y].type == KINGDOM  ||
        map[x][y + 1].type == KINGDOM  ||
        map[x][y - 1].type == KINGDOM) {
        if (map[x][y].type == TERRAIN) return 1;
    }
    if(map[x + 1][y].type == VILLAGE) {
        if (conectionvillage(x+1, y)) {
            if (map[x][y].type == TERRAIN) return 1;
        }
    }
    if(map[x - 1][y].type == VILLAGE) {
        if (conectionvillage(x-1, y)) {
            if (map[x][y].type == TERRAIN) return 1;
        }
    }
    if(map[x][y + 1].type == VILLAGE) {
        if (conectionvillage(x, y+1)) {
            if (map[x][y].type == TERRAIN) return 1;
        }
    }
    if(map[x][y - 1].type == VILLAGE) {
        if (conectionvillage(x, y-1)) {
            if (map[x][y].type == TERRAIN) return 1;
        }
    }
    return 0;
}

void ShowTextFornSecond(double n, const char *text, int x, int y, int player) 
{
    double startTime = GetTime();
    while(GetTime() - startTime < 0.2) {
        BeginDrawing();
        DrawMap(x,y,player);
        DrawText(text, 100, x*TILE_SIZE+120, 20, RED);
        EndDrawing();
    }
    while (GetTime() - startTime < n && !IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        BeginDrawing();
        DrawMap(x,y,player);
        DrawText(text, 100, x*TILE_SIZE+120, 20, RED);
        EndDrawing();
    }
}

int update(int x, int y, int player) {
    int prevGold = c[player].gold;
    int prevFood = c[player].food;
    int prevWorker = c[player].worker;
    int prevSoldier = c[player].soldier;

    DrawRectangle(90, 15, 120, 30, YELLOW);
    DrawRectangleLines(90, 15, 120, 30, BLACK);
    DrawText("buy food", 100, 20, 20, BLACK);
    DrawRectangle(90, 65, 130, 30, YELLOW);
    DrawRectangleLines(90, 65, 130, 30, BLACK);
    DrawText("buy worker", 100, 70, 20, BLACK);
    DrawRectangle(390, 15, 125, 30, YELLOW);
    DrawRectangleLines(390, 15, 125, 30, BLACK);
    DrawText("buy soldier", 400, 20, 20, BLACK);
    DrawRectangle(390, 65, 120, 30, YELLOW);
    DrawRectangleLines(390, 65, 120, 30, BLACK);
    DrawText("build road", 400, 70, 20, BLACK);
    DrawRectangle(240, 40, 120, 30, YELLOW);
    DrawRectangleLines(240, 40, 120, 30, BLACK);
    DrawText("do nothing", 250, 45, 20, BLACK);
    DrawPlayerInfo(c, y, x, player);
    DrawMap(x, y, player);

    while (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON));

    int a = 0;
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (mouseX >= 90 && mouseX <= 210 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 0) {
                ShowTextFornSecond(1, "succesfully!", x, y, player);
                c[player].gold--;
                c[player].food++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!", x, y, player);
                c[player].gold = prevGold;
                c[player].food = prevFood;
            }
        }
        else if (mouseX >= 90 && mouseX <= 210 && mouseY >= 65 && mouseY <= 95) {
            if (c[player].food > 2) {
                ShowTextFornSecond(1, "succesfully!", x, y, player);
                c[player].food -= 3;
                c[player].worker++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough food!", x, y, player);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].worker = prevWorker;
            }
        }
        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 15 && mouseY <= 45) {
            if (c[player].gold > 1) {
                ShowTextFornSecond(1, "succesfully!", x, y, player);
                c[player].gold -= 2;
                c[player].soldier++;
                a = 1;
            } else {
                ShowTextFornSecond(1, "Not enough gold!", x, y, player);
                c[player].gold = prevGold;
                c[player].food = prevFood;
                c[player].soldier = prevSoldier;
            }
        }
    
        else if (mouseX >= 390 && mouseX <= 510 && mouseY >= 65 && mouseY <= 95) {
            buildRoadMode = 1; 
            ShowTextFornSecond(20, "click to build road", x, y, player);
        }
        
        else if (mouseX >= 240 && mouseX <= 360 && mouseY >= 40 && mouseY <= 70){
            a = 1;
        }

        
        if (buildRoadMode) {
            Vector2 mousePosition = GetMousePosition();
            int gridX = (mousePosition.x / TILE_SIZE) - 2;
            int gridY = (mousePosition.y / TILE_SIZE) - 2;
            if (gridX >= 0 && gridX < x && gridY >= 0 && gridY < y) {
                if (canBuild(gridX, gridY)) {
                    if (map[gridX][gridY].difficulty[player] <= c[player].worker) {
                        map[gridX][gridY].type = ROAD;
                        map[gridX][gridY].forkingdom = player;
                    } else {
                        map[gridX][gridY].difficulty[player] -= c[player].worker;
                    }
                    buildRoadMode = 0;
                    a = 1;
                }
                else {
                    ShowTextFornSecond(1, "Cant build road here!", x, y, player);
                }
            }
        }
    }

    // افزودن این کد برای اطمینان از این‌که اگر کاری انجام شود، منابع به روز رسانی می‌شوند
    if (a) {
        c[player].gold += c[player].makeGold;
        c[player].food += c[player].makeFood;
    }

    return a;
}
