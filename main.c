#include <stdio.h>
#include <stdlib.h>
#include "printmap.h"
#include "generateMap.h"
#include "updategame.h"
#include "gameInformation.h"
#include "setupScreen.h"
#include "raylib.h"  

kingdom c[4];
Tile map[17][17];
int kingnum;
int alivePlayers;
int x, y;
double gameStartTime;
int minutes , seconds;
Texture2D background;
Sound lose;
Sound bubble;
Sound wrong;
Sound click;
Sound ding;
Sound badLuck;
Sound fight;

const int maxv=10;

int main() {
  InitAudioDevice();
  lose = LoadSound("sounds/kharabkardi.wav");
  Sound Win = LoadSound("sounds/Win.wav");
  bubble = LoadSound("sounds/bullb.wav");
  wrong = LoadSound("sounds/wrong.wav");
  click = LoadSound("sounds/click.mp3");
  ding = LoadSound("sounds/ding.wav");
  badLuck = LoadSound("sounds/badluck.mp3");
  fight = LoadSound("sounds/fight.mp3");

  InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "game");
  SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0)); 
  SetTargetFPS(60);

  background = LoadTexture("image/background.jpg");

  // ---------- صفحه‌ی ورودی گرافیکی (جایگزین منوی ترمینال) ----------
  int player = 0;
  SetupResult setupResult = SETUP_RESULT_NONE;
  while (!WindowShouldClose() && setupResult == SETUP_RESULT_NONE) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    setupResult = UpdateSetupScreen();
    EndDrawing();
  }

  if (WindowShouldClose()) {
    CloseWindow();
    CloseAudioDevice();
    return 0;
  }

  if (setupResult == SETUP_RESULT_LOAD) {
    player = GetLoadedPlayerTurn();
    if (player < 0) player = 0;
  }

  gameStartTime = GetTime();

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
      
      PlaySound(Win); 
      while(GetTime() - startTime < 2) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        sprintf(info, "Player %d Win!", plwin+1);
        DrawText(info, 150, 150, 50, color);
        EndDrawing();
      }
      break;
    }

    for(; player < kingnum; player++) {
        if(c[player].isAlive) {
      
        int a = 0;
        BeginDrawing(); 
        ClearBackground(RAYWHITE);
        
        a = update(player);
        if(a) savegame(player);
        else player--;
        if(a==2){
          CloseWindow();
          break;
        }
        EndDrawing();
        }
    }
    player = 0;
  }

  CloseWindow();
  CloseAudioDevice();

  return 0;
} 
