
#ifndef GENERATEARRAY_H
#define GENERATEARRAY_H


typedef struct{
    int x, y;
    int makeFood, makeGold;
}village;

typedef struct{
    int x, y;
    int makeFood, makeGold;
    int gold, soldier, food, worker; 
    village villgeconected[10]; 
    int villcount;
}kingdom;


typedef struct{
    int x, y;
}roadBuild;


#define INF 1000000

void enableRawMode();
void generate_array(int x, int y);
#endif 