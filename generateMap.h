#ifndef GENERATEMAP_H
#define GENERATEMAP_H

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
    int price_soldier , price_worker;
    int is_computer;
}kingdom;

typedef struct{
    Tile map[17][17];
    kingdom c[4];
    int kingnum;
    int aliveplayers;
    int x,y;
    int minutes , seconds;
}Game;

#define INF 1000000

void generate_array();

#endif