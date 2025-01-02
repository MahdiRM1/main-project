#include <stdio.h>
#include <unistd.h>
#include "updategame.h"
#include "generateArray.h"
#include "printmap.h"

extern kingdom c;
extern village Vill[10];
extern int map[17][17];
extern int v;
extern int x, y;
extern roadBuild road;

int canBuild(){
    if(road.x < 0 || road.x >= x || road.y < 0 || road.y >= y || map[road.x][road.y] == -2 || map[road.x][road.y] == 7) return 0;
    return 1;
}

int conectionvillage(village vill){//check to if village connect to kingdom
    if((map[(vill.x)+1][vill.y]==0) || (map[vill.x][(vill.y)+1]==0) || (map[vill.x][(vill.y)-1])==0 || (map[(vill.x)-1][vill.y]==0)) {
        return 1;
    }
    return 0;
}

void moveOnRoad(){
    while (map[road.x][road.y] == 0 || map[road.x][road.y] == 7 || map[road.x][road.y] == 8){
        clrscr(); printMap(); printAsset(v);
        char moveDirection;
        printf("enter next location with w.a.s.d:");
        scanf("%c", &moveDirection);
        switch (moveDirection){
            case 'W':
            case 'w': {
                road.x--;
                break;
            }
            case 'S':
            case 's': {
                road.x++;
                break;
            }
            case 'A':
            case 'a': {
                road.y--;
                break;
            }
            case 'D':
            case 'd': {
                road.y++;
                break;
            }
            default: printf("This is a wrong move.\n");
        }
    }
}

int roadBuilder(){
    if(!canBuild()){
		printf("you can't build this road!\nPlease press enter and try again!\n");
        sleep(1);
        return 0;
    }
    else{
        if (map[road.x][road.y] == 0 || map[road.x][road.y] == 7 || map[road.x][road.y] == 8) moveOnRoad();
        if(c.worker>=map[road.x][road.y]) {
        map[road.x][road.y] = 0;
        return 1;
        }
	    else{
		    map[road.x][road.y]-=c.worker;
            return 2;
	    }
    }
}

int update(){
    int action;
    clrscr();printMap();printAsset(v);printMove();
	scanf("%d", &action);
	switch(action){
		case 1:if(c.gold>0){c.food++; c.gold--; return 0;}
                return 1;
                break;
		case 2: if (c.food>2){c.worker++; c.food -= 3; return 0;}
                else{
                    printf("you don't have enough gold!\nPlease press enter and try again\n");
                    sleep(1);
                    return 1;
                }
                break;
		case 3: if(c.gold>1){c.soldier++; c.gold -=2; return 0;}
                else{
                    printf("you don't have enough gold!\nPlease press enter and try again\n");
                    sleep(1);
                    return 1;
                }
                    break;
		case 4:{
            int move;
            printf("Now enter the direction with w.a.s.d:");
            char jahat[2];
			scanf("%s", jahat);
			switch (jahat[0]){
                case 'W':
				case 'w':{
					road.x--;
                    move = roadBuilder();
					switch (move){
                    case 0:{
                        road.x++;
                        return 1;
                        break;
                    }
                    case 1:{
                        return 0;
                        break;
                    }
                    case 2:{
                        road.x++;
                        return 0;
                        break;
                    }
                    break;
				    }
                }
                case 'S':
				case 's':{
					road.x++;
                    move = roadBuilder();
					switch (move){
                    case 0:{
                        road.x--;
                        return 1;
                        break;
                    }
                    case 1:{
                        return 0;
                        break;
                    }
                    case 2:{
                        road.x--;
                        return 0;
                        break;
                    }
                    break;
				    }
                }
                case 'A':
				case 'a':{
		    		road.y--;
                    move = roadBuilder();
					switch (move){
                    case 0:{
                        road.y++;
                        return 1;
                        break;
                    }
                    case 1:{
                        return 0;
                        break;
                    }
                    case 2:{
                        road.y++;
                        return 0;
                        break;
                    }
                    break;
				    }
                }
                case 'D':
                case 'd':{
					road.y++;
                    move = roadBuilder();
					switch (move){
                    case 0:{
                        road.y--;
                        return 1;
                        break;
                    }
                    case 1:{
                        return 0;
                        break;
                    }
                    case 2:{
                        road.y--;
                        return 0;
                        break;
                    }
                    break;
				    }
                }
            }
        }
            case 5:{
                    return 0;
                    break;
                }
			default:printf("action not found\n");
	}
}