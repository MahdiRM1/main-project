#include <stdio.h>
#include "generateMap.h"
#include "printmap.h"
#include "raylib.h"
#include "info.h" 

extern kingdom c[4];
extern int kingnum;

void sleep(double n, int player) {
    BeginDrawing();
    DrawMap(player);
    DrawPlayerInfo(player);
    DrawMove(player);
    EndDrawing();
    WaitTime(n);  // این تابع خود Raylibه، از سیستم می‌خوابه، CPU مصرف نمی‌کنه
}

void drawX(int startX, int startY, int size, int thickness) {
    DrawLineEx((Vector2){startX, startY}, (Vector2){startX + size, startY + size}, thickness, BLACK);
    DrawLineEx((Vector2){startX + size, startY}, (Vector2){startX, startY + size}, thickness, BLACK);
}

void DrawPlayerInfo(int plturn) {
    Color color;
    char info[256];

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int heightrec = (screenHeight-40)/4;
    int startY = screenWidth - heightrec;
    
    
    for (int player = 0; player < kingnum; player++){
        color = checkColor(player);
        if(player != plturn) color = DARKGRAY;

        DrawRectangle(startY - 10, player*heightrec, heightrec + 10, heightrec, color);
        DrawRectangleLines(startY - 10, player*heightrec, heightrec + 10, heightrec, WHITE);

        sprintf(info, "Player %d", player + 1);
        DrawText(info, startY , player*heightrec + 30, 30, WHITE);

        sprintf(info, "Gold: %d", c[player].gold);
        DrawText(info, startY , player*heightrec + 65, 20, WHITE);

        sprintf(info, "Food: %d", c[player].food);
        DrawText(info, startY , player*heightrec + 90, 20, WHITE);

        sprintf(info, "Workers: %d (P: %d)", c[player].worker , c[player].price_worker);
        DrawText(info, startY , player*heightrec + 115, 20, WHITE);

        sprintf(info, "Soldiers: %d (P: %d)", c[player].soldier, c[player].price_soldier);
        DrawText(info, startY , player*heightrec + 140, 20, WHITE);

        sprintf(info, "Food rate: %d", c[player].FoodRate);
        DrawText(info, startY , player*heightrec + 165, 20, WHITE);

        sprintf(info, "Gold rate: %d", c[player].GoldRate);
        DrawText(info, startY , player*heightrec + 190, 20, WHITE);

        if(player > -1 && c[player].isAlive == 0) drawX(startY - 10, player*heightrec, heightrec, 20);
    }
}

void ShowTextFornSecond(double n, const char *text, int player, int ax, int ay, int size, Color color) {
    BeginDrawing();
    DrawMap(player);
    DrawMove(player);
    DrawPlayerInfo(player);
    DrawText(text, ax, ay, size, color);
    EndDrawing();

    double startTime = GetTime();
    while (GetTime() - startTime < n) {
        PollInputEvents(); // آپدیت وضعیت input بدون رندر کامل
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) break;
        WaitTime(0.01);
    }
}



void DrawMove(int player){
    
    int screenHeight = GetScreenHeight();
    int startY = 20;
    int startX = 200;

    Color color;
    color = checkColor(player);

    DrawRectangle(startY , startX , 200, 50, color);
    DrawRectangleLines(startY , startX, 200, 50, WHITE);
    DrawText("buy food", startY + 20, startX + 10, 25, WHITE);
    startX += 100;

    DrawRectangle(startY , startX, 200, 50, color);
    DrawRectangleLines(startY, startX, 200, 50, WHITE);
    DrawText("buy worker", startY + 20, startX + 10, 25, WHITE);
    startX += 100;

    DrawRectangle(startY, startX, 200, 50, color);
    DrawRectangleLines(startY, startX, 200, 50, WHITE);
    DrawText("buy soldier", startY + 20, startX + 10, 25, WHITE);
    startX += 100;

    DrawRectangle(startY, startX, 200, 50, color);
    DrawRectangleLines(startY, startX, 200, 50, WHITE);
    DrawText("do nothing", startY + 20, startX + 10, 25, WHITE);
}