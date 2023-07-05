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
    value[0][0] += 9, value[0][player->col_cnt - 1] += 9;
    value[player->row_cnt - 1][0] += 9, value[player->row_cnt - 1][player->col_cnt - 1] += 9;
    // 角的相邻位置减权
    value[0][1] -= 1, value[1][0] -= 2;
    value[0][player->col_cnt - 2] -= 2, value[1][player->col_cnt - 1] -= 2;
    value[player->row_cnt - 2][0] -= 2, value[player->row_cnt - 1][1] -= 2;
    value[player->row_cnt - 2][player->col_cnt - 1] -= 2, value[player->row_cnt - 1][player->col_cnt - 2] -= 2;
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
            if (is_valid(player, i, j, my_piece))
                h++;
            else if (is_valid(player, i, j, op_piece))
                h--;
        }
    }
    return h;
}
static int dfs(struct Player *player, int maxdepth, char my_piece, int beta)
{
    if (maxdepth == 0)
    {
        return h(player, my_piece);
    }
    int maxh = -INF;
    char op_piece = my_piece == 'O' ? 'o' : 'O';
    int queue[145][2], qp = 0;
    memset(queue, 0, sizeof(queue));
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, my_piece))
            {
                queue[qp][0] = i, queue[qp++][1] = j;
            }
        }
    }
    int max_tries = (qp >> 1) + 1;
    int cnt = 0;
    while (cnt <= max_tries && qp > 0)
    {
        int i;
        i = rand() % qp;
        int x = queue[i][0], y = queue[i][1];
        for (int j = i; j < qp - 1; j++)
        {
            queue[j][0] = queue[j + 1][0];
            queue[j][1] = queue[j + 1][1];
        }
        qp--;
        char c = player->mat[x][y];
        player->mat[x][y] = my_piece;
        int h1 = -(dfs(player, maxdepth - 1, op_piece, -maxh));
        if (h1 != -INF && h1 != INF && h1 > maxh)
        {
            cnt = 0;
            maxh = h1;
        }
        else
        {
            cnt++;
        }
        player->mat[x][y] = c;
    }
    return maxh;
}
struct Point place(struct Player *player)
{
    int maxh = -INF, maxx = -1, maxy = -1;
    int queue[145][2], qp = 0;
    memset(queue, 0, sizeof(queue));
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, 'O'))
            {
                queue[qp][0] = i, queue[qp++][1] = j;
            }
        }
    }
    int max_tries = (qp >> 1) + 1;
    int cnt = 0;
    while (cnt <= max_tries && qp > 0)
    {
        int i;
        i = rand() % qp;
        int x = queue[i][0], y = queue[i][1];
        for (int j = i; j < qp - 1; j++)
        {
            queue[j][0] = queue[j + 1][0];
            queue[j][1] = queue[j + 1][1];
        }
        qp--;
        char c = player->mat[x][y];
        player->mat[x][y] = 'O';
        int h1 = -dfs(player, 4, 'o', -maxh);
        if (h1 != -INF && h1 != INF && h1 > maxh)
        {
            cnt = 0;
            maxh = h1;
            maxx = x;
            maxy = y;
        }
        else
        {
            cnt++;
        }
        player->mat[x][y] = c;
    }
    if (maxx != -1 || maxy != -1)
        return initPoint(maxx, maxy);
    return initPoint(-1, -1); // give up
}
