#include <stdio.h>
#include <stdlib.h>
#include "printmap.h"
#include "generateArray.h"
#include "raylib.h"

extern Tile map[17][17];

void clrscr(){
    system("clear");
}

Color checkColor(int player) {
    Color color;
    switch (player) {
        case -1: color = GREEN; break;
        case 0: color = BLUE; break;
        case 1: color = RED; break;
        case 2: color = ORANGE; break;
        case 3: color = GOLD; break;
    }
    return color;
}

void DrawMap(int x, int y, int player) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            int positionX = i * TILE_SIZE + 100;
            int positionY = j * TILE_SIZE + 100;
            Color color = LIGHTGRAY;

            switch (map[i][j].type) {      
                case VILLAGE: {
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "V"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, BLACK); break;
                }
                case KINGDOM:{
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "C"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, BLACK); break;
                }
                case BLOCK_HOUSE: color = BLACK; DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);
                    char valueText[10];
                    snprintf(valueText, 10, "X"); 
                    DrawText(valueText, 
                        positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                        positionY + TILE_SIZE / 2 - 10, 20, WHITE); break;
                    break;
                case TERRAIN:color = BROWN; DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color);  break;
                case ROAD:
                    color = checkColor(map[i][j].forkingdom);
                    DrawRectangle(positionX, positionY, TILE_SIZE, TILE_SIZE, color); break;
                default: break;
            }
            DrawRectangleLines(positionX, positionY, TILE_SIZE, TILE_SIZE, BLACK);

            if(map[i][j].type == TERRAIN){
            char valueText[10]; 
            snprintf(valueText, 10, "%d", map[i][j].difficulty[player]); 
            DrawText(valueText, 
                    positionX + TILE_SIZE / 2 - MeasureText(valueText, 20) / 2, 
                    positionY + TILE_SIZE / 2 - 10, 20, BLACK);
            }
        }
    }
}