#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <malloc.h>
#define l 8

struct Point {
    int X, Y;
};
static const int MAXN = 12;
static int value[MAXN][MAXN];
char start[10][10]=
{"11111111",
"11111111",
"11111111",
"111oO111",
"111Oo111",
"11111111",
"11111111",
"11111111",
};

struct Player {
    char **mat;  // Game Data

    int row_cnt;  // Count of rows in current map

    int col_cnt;  // Count of columns in current map

    int your_score;  // Your AI's score

    int opponent_score;  // Opponent AI's score
};

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if ('1' <= player->mat[i][j] && player->mat[i][j] <= '9')
            {
                value[i][j] = player->mat[i][j] - '0';
            }
            else
            {
                value[i][j] = 0;
            }
        }
    }
    value[0][0] += 9, value[0][player->col_cnt - 1] += 9;
    value[player->row_cnt - 1][0] += 9, value[player->row_cnt - 1][player->col_cnt - 1] += 9;
    // 角的相邻位置减权
    value[0][1] -= 1, value[1][0] -= 1;
    value[0][player->col_cnt - 2] -= 1, value[1][player->col_cnt - 1] -= 1;
    value[player->row_cnt - 2][0] -= 1, value[player->row_cnt - 1][1] -= 1;
    value[player->row_cnt - 2][player->col_cnt - 1] -= 1, value[player->row_cnt - 1][player->col_cnt - 2] -= 1;
}

int check(struct Player *player, char **s, struct Point point, int num){
	if(num==2){
		return 0;
	}
	else if(num==0){///////////////////////
		int count=0;
	int posx=point.X;
	int posy=point.Y;
	char **tmp=(char**)malloc(player->row_cnt*sizeof(char*));
    for(int i=0;i<player->row_cnt;i++){
        tmp[i]=(char*)malloc(player->col_cnt*sizeof(char));
    }
	for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            tmp[i][j]=s[i][j];
        }
    }
	tmp[posx][posy]='O';
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (tmp[x][y] != 'o') {
            continue;
        }
        int flag=0;
        while (flag<15) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (tmp[x][y] >= '1' && tmp[x][y] <= '9')) {
                break;
            }
            if (tmp[x][y] == 'O') {
                x=posx + step[dir][0];
                y=posy + step[dir][1];
                while(tmp[x][y]!='O'){
                	tmp[x][y]='O';
                	x += step[dir][0];
            		y += step[dir][1];
				}
            }
            flag++;
        }
    }
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (tmp[i][j] == 'O') {
                count=count+value[i][j];
                struct Point ptmp;
                ptmp.X=i;ptmp.Y=j;
                count=count+check(player,tmp,ptmp,num+1);
            }
            else if (tmp[i][j] == 'o') {
                count=count-value[i][j];
                struct Point ptmp;
                ptmp.X=i;ptmp.Y=j;
                count=count+check(player,tmp,ptmp,num+1);
            }
        }
    }
    
    for(int i=0;i<player->row_cnt;i++){
        free(tmp[i]);
    }
    free(tmp);
    return count;
	}
	else if(num==1){//////////////
	int count=0;
	int posx=point.X;
	int posy=point.Y;
	char **tmp=(char**)malloc(player->row_cnt*sizeof(char*));
    for(int i=0;i<player->row_cnt;i++){
        tmp[i]=(char*)malloc(player->col_cnt*sizeof(char));
    }
	for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            tmp[i][j]=s[i][j];
        }
    }
	tmp[posx][posy]='o';
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (tmp[x][y] != 'O') {
            continue;
        }
        int flag=0;
        while (flag<15) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (tmp[x][y] >= '1' && tmp[x][y] <= '9')) {
                break;
            }
            if (tmp[x][y] == 'o') {
                x=posx + step[dir][0];
                y=posy + step[dir][1];
                while(tmp[x][y]!='o'){
                	tmp[x][y]='o';
                	x += step[dir][0];
            		y += step[dir][1];
				}
            }
            flag++;
        }
    }
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (tmp[i][j] == 'o') {
                count=count+value[i][j];
            }
            else if (tmp[i][j] == 'O') {
                count=count-value[i][j];
            }
        }
    }
    
    for(int i=0;i<player->row_cnt;i++){
        free(tmp[i]);
    }
    free(tmp);
    return count;
	}
}

int is_valid(struct Player *player, int posx, int posy) {
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt) {
        return false;
    }
    if (player->mat[posx][posy] == 'o' || player->mat[posx][posy] == 'O') {
        return false;
    }
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (player->mat[x][y] != 'o') {
            continue;
        }
        while (true) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9')) {
                break;
            }
            if (player->mat[x][y] == 'O') {
                return true;
            }
        }
    }
    return false;
}


struct Point place(struct Player *player) {
    struct Point *ok_points = (struct Point *)malloc((player->row_cnt * player->col_cnt) * sizeof(struct Point));
    int ok_cnt = 0;
	for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            if (is_valid(player, i, j)) {
                ok_points[ok_cnt].X=i;
                ok_points[ok_cnt++].Y=j;
            }
        }
    }
    struct Point point;
    point.X=-1;
    point.Y=-1;
    int max=-1000;
    if (ok_cnt > 0) {
    	for(int i=0;i<ok_cnt;i++){
    		int maxn=check(player,player->mat,point[i],0);
    		if(maxn>max){
    			point=ok_points[i];
    			max=maxn;
			}
		}
    }
    free(ok_points);
	return point;
}

void pprint(struct Point p){
	printf("(%d,%d)\n",p.X+1,p.Y+1);
}

void add1(struct Point p,struct Player *player){
	int posx=p.X;
	int posy=p.Y;	
	player->mat[posx][posy]='O';
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (player->mat[x][y] != 'o') {
            continue;
        }
        int flag=0;
        while (flag<15) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9')) {
                break;
            }
            if (player->mat[x][y] == 'O') {
                x=posx + step[dir][0];
                y=posy + step[dir][1];
                while(player->mat[x][y]!='O'){
                	player->mat[x][y]='O';
                	x += step[dir][0];
            		y += step[dir][1];
				}
            }
            flag++;
        }
    }
}

void add2(struct Point p,struct Player *player){
	int posx=p.X;
	int posy=p.Y;	
	player->mat[posx][posy]='o';
	int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0;  dir < 8; dir++) {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt) {
            continue;
        }
        if (player->mat[x][y] != 'O') {
            continue;
        }
        while (true) {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt || (player->mat[x][y] >= '1' && player->mat[x][y] <= '9')) {
                break;
            }
            if (player->mat[x][y] == 'o') {
                x=posx + step[dir][0];
                y=posy + step[dir][1];
                while(player->mat[x][y]!='o'){
                	player->mat[x][y]='o';
                	x += step[dir][0];
            		y += step[dir][1];
				}
            }
        }
    }
}

int main(){
	struct Player *player;
	player=(Player*)malloc(sizeof(Player));
	player->row_cnt=8;player->col_cnt=8;
	player->mat=(char**)malloc(10*sizeof(char*));
	char *tmp;

	for(int i=0;i<10;i++){
		tmp=(char*)malloc(10*sizeof(char));
		player->mat[i]=tmp;
	}
	for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            player->mat[i][j]=start[i][j];
        }
    }
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            printf("%c",player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
	init(player);    
o:  struct Point P=place(player);
    add1(P,player);
    for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            printf("%c",player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pprint(P);
    struct Point p1;
	scanf("%d %d",&p1.X,&p1.Y);
	p1.X-=1;
	p1.Y-=1; 
	add2(p1,player);
	for (int i = 0; i < player->row_cnt; i++) {
        for (int j = 0; j < player->col_cnt; j++) {
            printf("%c",player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    goto o;
    return 0;
}
