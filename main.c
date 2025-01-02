#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printmap.h"
#include "generateArray.h"
#include "updategame.h"

kingdom c;
village Vill[10];
int map[17][17]; //main array
int x, y;
roadBuild road = {-1, -1};
int v;

const int maxv=10;

int main() {
	clrscr();
		int n, i;
		printf("enter x and y for map:"); scanf("%d %d", &x, &y);
		generate_array(x,y);
		printf("enter location of kingdom:"); scanf("%d %d", &c.x, &c.y);
		c.x--; c.y--; c.makeGold=1; c.makeFood=0; c.gold=5; c.food=0; c.worker=1; c.soldier=0; c.villcount=0;
		map[c.x][c.y] = 7;

		printf("enter number of villages:");
		scanf("%d", &v);
		for(i=0; i<v; i++) {
			printf("enter location of village %d:", i+1);
			scanf("%d %d", &Vill[i].x, &Vill[i].y);
			Vill[i].x--; Vill[i].y--;
			printf("enter food generation rate and gold generation rate for village %d:", i+1);
			scanf("%d %d", &Vill[i].makeFood, &Vill[i].makeGold);
			map[Vill[i].x][Vill[i].y] = 8;
		}

		printf("enter number of blocked houses:");
		scanf("%d", &n);
		for(i=0; i<n; i++) {
			int pi, qi;
			printf("enter location of blocked house %d:", i+1);
			scanf("%d %d", &pi, &qi);
			map[pi-1][qi-1] = -2;
		}

		i = 0;
		int villcount;
		clrscr();printMap();printAsset(v);printMove();
		sleep(1);
		road.x = c.x; road.y = c.y;

		do{
			int up = 1;
			while(up){
			up = update();
			}
			c.gold+=c.makeGold;
			c.food+=c.makeFood;
			for(i=0;i<v;i++){
				if(conectionvillage(Vill[i])){
					c.gold+=Vill[i].makeGold;
					c.food+=Vill[i].makeFood;
				}
			}
			villcount = 0;
			for(i=0;i<v;i++){
				if(conectionvillage(Vill[i])) villcount++;
			}
		}while(villcount < v);
		clrscr(); printMap();
		printf("YOU WIN!\n");

		printf("press any key to exit\n");
		scanf("%d", &i);

		return 0;
	}
