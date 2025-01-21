#ifndef WAR_H
#define WAR_H

typedef struct{
    int x, y;
    int player;
}fighter;

void War(int gridX, int gridY, int player);

#endif