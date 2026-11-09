#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "updategame.h"
#include "generateMap.h"
#include "printmap.h"
#include "info.h" 
#include "War.h"
#include "Computer.h"

extern kingdom c[4];
extern int kingnum;
extern Tile map[17][17];
extern int x, y;

int roadbuild_computer(int player){
    for(int dif = 1 ; dif < 5 ; dif++){
        for (int i = 0; i < x; i++){
            for (int j = 0; j < y; j++) {
                if (map[i][j].forkingdom != player) {
                    if (map[i][j].difficulty[player] == dif){
                        if(canBuild(i , j , player)){
                            if (map[i][j].difficulty[player] <= c[player].worker) {
                                map[i][j].type = ROAD;
                                map[i][j].forkingdom = player;
                                War(i, j, player);
                                if(map[i][j].forkingdom == player)
                                    for(int k=0; k<3; k++)
                                        conectionvillage(i, j, player);
                            } else {
                                map[i][j].difficulty[player] -= c[player].worker;
                            }
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;        
}

int computer(int player){//game with computer
    int random = rand() % 100;
    Color color = checkColor(player);
    if (random < 40){
        //buy soldier
        if (c[player].gold >=c[player].price_soldier) {
            ShowTextFornSecond(1, "Buy soldier successfully!", player, 100, 100, 20, color);
            c[player].gold -= c[player].price_soldier;
            c[player].soldier++;
            c[player].price_soldier++;
            return 1;
        } else {
            return 0;
        }
    }
    else if(random < 45){
        // Buy food
        if(c[player].food > 3) return 0 ;
        if (c[player].gold > 0) {
            ShowTextFornSecond(1, "Buy food successfully!", player, 100, 100, 20, color);
            c[player].gold--;
            c[player].food++;
            return 1;
        } else {
            return 0;
        }
    }
    else if (random < 55){
        // Buy worker
        if(c[player].worker > 3) return 0;
        if (c[player].food >= c[player].price_worker) {
            ShowTextFornSecond(1, "Buy worker successfully!", player, 100, 100, 20, color);
            c[player].food -= c[player].price_worker;
            c[player].worker++;
            c[player].price_worker++;
            return 1;
        } else {
            return 0;
        }
    }
    else{
        ShowTextFornSecond(1, "Build road successfully!", player, 100, 100, 20, color);
        int d = roadbuild_computer(player);
        return  d;
    }
}