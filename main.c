#include <stdio.h>
#include <stdlib.h>
#include "printmap.h"
#include "generateArray.h"
#include "updategame.h"
#include "raylib.h"  

kingdom c[4];
Tile map[17][17]; //main array
int v;
int kingnum;
int alivePlayers;
int x, y;

const int maxv=10;

int main() {
  clrscr();
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
    c[i].x--; c[i].y--; c[i].GoldRate=1; c[i].FoodRate=0; c[i].gold=5; c[i].food=0; c[i].worker=1;
    c[i].soldier=0; map[c[i].x][c[i].y].forkingdom = i; c[i].isAlive = 1; map[c[i].x][c[i].y].type = KINGDOM;
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
  
  int screenWidth = x * TILE_SIZE;
  int screenHeight = y * TILE_SIZE;

  InitWindow(screenWidth, screenHeight, "game");
  SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0)); 
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    if(alivePlayers < 2) {
      int plwin;
      for(int i = 0; i < kingnum; i++) {
        if(c[i].isAlive) plwin = i;
      }
      Color color;
      color = checkColor(plwin);
      char info[20]; 
      double startTime = GetTime();
      while(GetTime() - startTime < 2) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        sprintf(info, "player %d win!", plwin+1);
        DrawText(info, 150, 150, 50, color);
        EndDrawing();
      }
      break;
    }
    for(int player = 0; player <kingnum; player++) {
        if(c[player].isAlive) {
      
        int a = 0;
        BeginDrawing(); 
        ClearBackground(RAYWHITE);
        
        a = update(player); 
        if (!a) {
            player--;
        }
        EndDrawing();
        }
    }
}

CloseWindow();

  return 0;
}