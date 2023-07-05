/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
static const int MAXN = 12;
static const int INF = 0x3f3f3f3f;
static int value[MAXN][MAXN];
void init(struct Player *player)
{
    srand(time(NULL));
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
            if (i == 0 || j == 0 || i == player->row_cnt - 1 || j == player->col_cnt - 1)
            {
                value[i][j] += 1;
            }
        }
    }
    // 四个角加权
    const int v1 = 15;
    value[0][0] += v1, value[0][player->col_cnt - 1] += v1;
    value[player->row_cnt - 1][0] += v1, value[player->row_cnt - 1][player->col_cnt - 1] += v1;
    // 角的相邻位置减权
    const int v2 = 5;
    value[0][1] -= v2, value[1][0] -= v2;
    value[0][player->col_cnt - 2] -= v2, value[1][player->col_cnt - 1] -= v2;
    value[player->row_cnt - 2][0] -= v2, value[player->row_cnt - 1][1] -= v2;
    value[player->row_cnt - 2][player->col_cnt - 1] -= v2, value[player->row_cnt - 1][player->col_cnt - 2] -= v2;
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
static void add(int posx, int posy, struct Player *player, char my_piece)
{
    player->mat[posx][posy] = my_piece;
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    char op_piece = my_piece == 'O' ? 'o' : 'O';
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
                x = posx + step[dir][0];
                y = posy + step[dir][1];
                while (player->mat[x][y] != my_piece)
                {
                    player->mat[x][y] = my_piece;
                    x += step[dir][0];
                    y += step[dir][1];
                }
                break;
            }
        }
    }
}
static int h(struct Player *player, char my_piece)
{
    int h = 0;
    char op_piece = my_piece == 'O' ? 'o' : 'O';
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
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, op_piece))
                h--;
        }
    }
    return h;
}
static int dfs(struct Player *player, int maxdepth, char my_piece, int alpha, int beta)
{
    if (maxdepth == 0)
    {
        return h(player, my_piece);
    }
    int maxh = -INF;
    char op_piece = my_piece == 'O' ? 'o' : 'O';
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, my_piece))
            {
                int x = i, y = j;
                char tmp[player->row_cnt][player->col_cnt + 1];
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        tmp[i][j] = player->mat[i][j];
                    }
                    tmp[i][player->col_cnt] = '\0';
                }
                add(x, y, player, my_piece);
                int h1 = -(dfs(player, maxdepth - 1, op_piece, -beta, -alpha));
                if (h1 >= maxh)
                {
                    maxh = h1;
                    if (maxh > alpha)
                        alpha = maxh;
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        player->mat[i][j] = tmp[i][j];
                    }
                }
                if (maxh > beta)
                    return maxh;
            }
        }
    }
    return maxh;
}
struct Point place(struct Player *player)
{
    int maxh = -INF, maxx = -1, maxy = -1;
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, 'O'))
            {
                int x = i, y = j;
                char tmp[player->row_cnt][player->col_cnt + 1];
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        tmp[i][j] = player->mat[i][j];
                    }
                    tmp[i][player->col_cnt] = '\0';
                }
                add(x, y, player, 'O');
                int maxdepth = 3;
                int h1 = -dfs(player, maxdepth, 'o', maxh, -maxh);
                if (h1 >= maxh)
                {
                    maxh = h1;
                    maxx = x;
                    maxy = y;
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        player->mat[i][j] = tmp[i][j];
                    }
                }
            }
        }
    }
    if (maxx != -1 || maxy != -1)
        return initPoint(maxx, maxy);
    return initPoint(-1, -1); // give up
}
