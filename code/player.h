/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
    srand(time(0));
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
                ok_points[ok_cnt++] = initPoint(i, j);
            }
        }
    }
    struct Point point = initPoint(-1, -1); 
    if (ok_cnt > 0) {
        int flag=0;
        for(int i=0;i<ok_cnt;i++)
        {
            if((ok_points[i].X==0&&ok_points[i].Y==0)||(ok_points[i].X==player->row_cnt-1&&ok_points[i].Y==player->col_cnt-1)||(ok_points[i].X==0&&ok_points[i].Y==player->col_cnt-1)||ok_points[i].X==player->row_cnt-1&&ok_points[i].Y==0)
            {
                point=ok_points[i];
                flag=1;
            }
            else if(ok_points[i].X>=player->row_cnt/2-2&&ok_points[i].Y>=player->col_cnt/2-2&&ok_points[i].X<=player->row_cnt/2+1&&ok_points[i].Y<=player->col_cnt/2+1)
            {
                point=ok_points[i];
                flag=1;
            }
            else if((ok_points[i].X==0&&ok_points[i].Y>=2&&ok_points[i].Y<=player->col_cnt-3)||(ok_points[i].Y==0&&ok_points[i].X>=2&&ok_points[i].X<=player->row_cnt-3)||(ok_points[i].X==player->col_cnt-1&&ok_points[i].Y>=2&&ok_points[i].Y<=player->col_cnt-3)||(ok_points[i].Y==player->row_cnt-1&&ok_points[i].X>=2&&ok_points[i].X<=player->col_cnt-3))
            {
                point=ok_points[i];
                flag=1;
            }
        }
        if(flag==0)
        {
            point = ok_points[rand() % ok_cnt];
        }
    }
    free(ok_points);
	return point;
}
