#include <stdio.h>
#include <stdlib.h>
#include "updategame.h"
#include "printmap.h"
#include "generateMap.h"
#include "raylib.h"

extern Tile map[17][17];
extern int x, y;
extern kingdom c[4];
extern double gameStartTime;
extern int minutes , seconds;
extern Texture2D background;
extern int agominutes , agoseconds;

void clrscr(){
    system("clear");
}

Color checkColor(int player) {
    Color color;
    switch (player) {
        case -1: color = GREEN; break;
        case 0: color = BLUE; break;
        case 1: color = RED; break;
        case 2: color = DARKGREEN; break;
        case 3: color = GOLD; break;
    }
    if(player > -1 && c[player].isAlive == 0) color = BLACK;
    return color;
} 


void DrawMap(int player) {
    Vector2 mousePosition = GetMousePosition();
    
    int centerX = (GetScreenWidth() - background.width) / 2; 
    int centerY = (GetScreenHeight() - background.height) / 2;
    DrawTexture(background, centerX , centerY , WHITE);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {

            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            int startX = (screenWidth - (x * TILE_SIZE)) / 2;
            int startY = (screenHeight - (y * TILE_SIZE)) / 2;
            int positionX = i * TILE_SIZE + startX;
            int positionY = j * TILE_SIZE + startY;

            Rectangle tileRect = { positionX, positionY, TILE_SIZE, TILE_SIZE };
            Color color = LIGHTGRAY;

            bool isHovered = CheckCollisionPointRec(mousePosition, tileRect);

            switch (map[i][j].type) {      
                case VILLAGE: {
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "V"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, WHITE); break;
                }
                case KINGDOM:{
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "C"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, WHITE); break;
                }
                case BLOCK_HOUSE: color = BLACK;
                DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "X"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, WHITE); break;
                    break;
                case TERRAIN:
                if (isHovered){
                    int gridX = (mousePosition.x - startX) / TILE_SIZE;
                    int gridY = (mousePosition.y - startY) / TILE_SIZE;
                    if(canBuild(gridX, gridY, player)){
                        color = DARKGRAY;
                        DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    }
                }
                break;
                case ROAD:
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color); break;
                default: break;
            }
            DrawRectangleLines(positionX, positionY, TILE_SIZE, TILE_SIZE, WHITE);

            if(map[i][j].type == TERRAIN){
            char valueText[10]; 
            snprintf(valueText, 10, "%d", map[i][j].difficulty[player]); 
            DrawText(valueText, 
                    positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                    positionY + TILE_SIZE / 2 - 10, 20, WHITE);
            }
        }
    }
        double elapsedTime = GetTime() - gameStartTime;
        minutes = (int)((elapsedTime + agominutes*60)/ 60) ;
        seconds = (int)(elapsedTime + agoseconds) % 60;
        char timerText[50];
        sprintf(timerText, "Time of game: %02d:%02d", minutes, seconds);   
        DrawRectangle(GetScreenWidth() - 500, 10, 200, 50, BLACK);
        DrawText(timerText, GetScreenWidth() - 495, 20, 20, WHITE);
}