/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include "../include/playerbase.h"
#include <stdio.h>
#include <string.h>
static const int MAXN = 12;
static int value[MAXN][MAXN];
void init(struct Player *player)
{
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
    // 四个角加权
    value[0][0] += 9, value[0][player->col_cnt - 1] += 9;
    value[player->row_cnt - 1][0] += 9, value[player->row_cnt - 1][player->col_cnt - 1] += 9;
    // 角的相邻位置减权
    value[0][1] -= 1, value[1][0] -= 1;
    value[0][player->col_cnt - 2] -= 1, value[1][player->col_cnt - 1] -= 1;
    value[player->row_cnt - 2][0] -= 1, value[player->row_cnt - 1][1] -= 1;
    value[player->row_cnt - 2][player->col_cnt - 1] -= 1, value[player->row_cnt - 1][player->col_cnt - 2] -= 1;
}
static int is_valid(struct Player *player, int posx, int posy, char my_piece)
{
    char op_piece = my_piece == 'O' ? 'o' : 'O';
    if (posx < 0 || posx >= player->row_cnt || posy < 0 || posy >= player->col_cnt)
    {
        return false;
    }
    if (player->mat[posx][posy] == op_piece || player->mat[posx][posy] == my_piece)
    {
        return false;
    }
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
        {
            continue;
        }
        if (player->mat[x][y] != op_piece)
        {
            continue;
        }
        while (true)
        {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt ||
                (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
            {
                break;
            }
            if (player->mat[x][y] == my_piece)
            {
                return true;
            }
        }
    }
    return false;
}
static int h(struct Player *player, int posx, int posy, char my_piece, int maxh)
{
    int h = 0;
    char op_piece = my_piece == 'O' ? 'o' : 'O';
    char c = player->mat[posx][posy];
    player->mat[posx][posy] = my_piece;
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (player->mat[i][j] == my_piece)
            {
                h += value[i][j];
            }
            else
            {
                if (player->mat[i][j] == op_piece)
                    h -= value[i][j];
            }
        }
    }
    if (h > maxh)
        for (int i = 0; i < player->row_cnt; i++)
        {
            for (int j = 0; j < player->col_cnt; j++)
            {
                if (is_valid(player, i, j, op_piece))
                {
                    h--;
                    if (h <= maxh)
                        break;
                }
            }
        }
    player->mat[posx][posy] = c;
    return h;
}
struct Point place(struct Player *player)
{
    int maxh = -0x3f3f3f3f, maxx = -1, maxy = -1;
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, 'O'))
            {
                int h1 = h(player, i, j, 'O', maxh);
                if (h1 > maxh)
                {
                    maxh = h1;
                    maxx = i;
                    maxy = j;
                }
            }
        }
    }
    if (maxx != -1 || maxy != -1)
        return initPoint(maxx, maxy);
    return initPoint(-1, -1); // give up
}
