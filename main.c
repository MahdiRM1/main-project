#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printmap.h"
#include "generateArray.h"
#include "updategame.h"
#include "raylib.h"  

kingdom c[4];
Tile map[17][17]; //main array
int v;
village Vill[10];
int kingnum;

const int maxv=10;

int main() {
  clrscr();
  int x, y;
  int n, i;
  printf("enter x and y for map:"); scanf("%d %d", &x, &y);
  generate_array(x,y);
  printf("enter number of kingdoms:");
  scanf("%d", &kingnum);
  for(i=0; i<kingnum; i++){
    printf("enter location of kingdom %d:", i+1); scanf("%d %d", &c[i].x, &c[i].y);
    c[i].x--; c[i].y--; c[i].makeGold=1; c[i].makeFood=0; c[i].gold=5; c[i].food=0; c[i].worker=1;
    c[i].soldier=0; c[i].villcount=0; map[c[i].x][c[i].y].forkingdom=i; map[c[i].x][c[i].y].type = KINGDOM;
  } 

  int v;
  village Vill[10];
  while(true){
    printf("enter number of villages:");
    scanf("%d", &v);
    if(v <= 10) break;
    else printf("number of villages should be less than 10\n");
  } 
    for(i=0; i<v; i++) {
      printf("enter location of village %d:", i+1);
      scanf("%d %d", &Vill[i].x, &Vill[i].y);
      Vill[i].x--; Vill[i].y--; map[Vill[i].x][Vill[i].y].forkingdom = -1;
      printf("enter food generation rate and gold generation rate for village %d:", i+1);
      scanf("%d %d", &Vill[i].makeFood, &Vill[i].makeGold);
      map[Vill[i].x][Vill[i].y].type = VILLAGE;
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
    bool skipPlayer = false; 
    
    for(int player = 0; player < kingnum; player++) {
        int a = 0;
        BeginDrawing(); 
        ClearBackground(RAYWHITE);
        
        a = update(x, y, player); 
        
        if (!a) {
            skipPlayer = true;
        }

        if (!skipPlayer) {
            DrawMap(x, y, player);
        }
        
        EndDrawing();
        
        if (skipPlayer) {
            continue;
        }
    }
}

CloseWindow();

test for pull
  return 0;
}