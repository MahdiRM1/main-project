#ifndef INFO_H
#define INFO_H

#include "raylib.h"

void sleep(double n, int player);
void DrawPlayerInfo(int plturn);
void ShowTextFornSecond(double n, const char *text, int player, int ax, int ay, int size, Color color);
void DrawMove(int player);

#endif