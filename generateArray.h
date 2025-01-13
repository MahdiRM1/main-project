#ifndef GENERATEARRAY_H
#define GENERATEARRAY_H

#define MAP_SIZE 17
#define TILE_SIZE 50

typedef enum {
    EMPTY,
    ROAD,
    VILLAGE,
    KINGDOM,
    BLOCK_HOUSE,
    TERRAIN
} TileType;

typedef struct {
    TileType type;
    int difficulty[4];
    int firstDiff;
    int forkingdom;
    int FoodRate, GoldRate;
} Tile;

typedef struct{
    int x, y;
    int FoodRate, GoldRate;
    int gold, soldier, food, worker;
    int isAlive;
}kingdom;


#define INF 1000000

void generate_array();
#endif 