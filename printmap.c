#include <stdio.h>
#include <stdlib.h>
#include "printmap.h"
#include "generateArray.h"
#include "updategame.h"

extern kingdom c;
extern village Vill[10];
extern int map[17][17];
extern roadBuild road;
extern int x, y;

void clrscr(){
    system("clear");
}

void printMap() {
    //function for print game map
    int i, j;
    for(i=0; i<x; i++) {
        for(j=0; j<y; j++) {
            if(i == road.x && j == road.y) printf("🟢");
            else if(map[i][j]==7) printf("🏰");
            else if(map[i][j]==-2) printf("❌");
            else if(map[i][j]==8) printf("🌳");
            else if(map[i][j]==0) printf("🛣 ");
            else if(map[i][j]==1) printf("1️⃣ ");
            else if(map[i][j]==2) printf("2️⃣ ");
            else if(map[i][j]==3) printf("3️⃣ ");
            else if(map[i][j]==4) printf("4️⃣ ");
        }
        printf("\n");
    }
    printf("------------------------------\n");
}


void printMove(){
    printf("If you want to buy food for 1 gold, enter 1.\nIf you want to buy the worker for 3 food, enter 2.\n");
    printf("If you want to buy the soldier for 2 gold, enter 3.\nif you want to build a road, enter 4.\nif you want to do nothing press 5.\nselect your action:\n");
}

void printAsset(int v){
    int cmakeGold=c.makeGold, cmakeFood=c.makeFood;
    int i;
    for(i=0;i<v;i++){
		if(conectionvillage(Vill[i])){
			cmakeGold+=Vill[i].makeGold;
			cmakeFood+=Vill[i].makeFood;
		}
    }
    printf("\nGold production rate:%d\n", cmakeGold);
    printf("food production rate:%d\n", cmakeFood);
    printf("food:%d\n", c.food);
    printf("gold:%d\n", c.gold);
    printf("soldier:%d\n", c.soldier);
    printf("worker:%d\n", c.worker);
    printf("------------------------------\n");

}
