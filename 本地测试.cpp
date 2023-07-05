#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define l 8

struct Point
{
    int X, Y;
};
static const int MAXN = 12;
static int value[MAXN][MAXN];
static const int INF = 0x3f3f3f3f;
char start[10][10] = {
    "11111111", "11111111", "11111111", "111oO111", "111Oo111", "11111111", "11111111", "11111111",
};

struct Player
{
    char **mat; // Game Data

    int row_cnt; // Count of rows in current map

    int col_cnt; // Count of columns in current map

    int your_score; // Your AI's score

    int opponent_score; // Opponent AI's score
};

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
    value[0][0] += 9, value[0][player->col_cnt - 1] += 9;
    value[player->row_cnt - 1][0] += 9, value[player->row_cnt - 1][player->col_cnt - 1] += 9;
    // �ǵ�����λ�ü�Ȩ
    value[0][1] -= 1, value[1][0] -= 1;
    value[0][player->col_cnt - 2] -= 1, value[1][player->col_cnt - 1] -= 1;
    value[player->row_cnt - 2][0] -= 1, value[player->row_cnt - 1][1] -= 1;
    value[player->row_cnt - 2][player->col_cnt - 1] -= 1, value[player->row_cnt - 1][player->col_cnt - 2] -= 1;
}

int check(struct Player *player, char **s, struct Point point, int num)
{
    if (num == 2)
    {
        return 0;
    }
    else if (num == 0)
    { ///////////////////////
        int count = 0;
        int posx = point.X;
        int posy = point.Y;
        char **tmp = (char **)malloc(player->row_cnt * sizeof(char *));
        for (int i = 0; i < player->row_cnt; i++)
        {
            tmp[i] = (char *)malloc(player->col_cnt * sizeof(char));
        }
        for (int i = 0; i < player->row_cnt; i++)
        {
            for (int j = 0; j < player->col_cnt; j++)
            {
                tmp[i][j] = s[i][j];
            }
        }
        tmp[posx][posy] = 'O';
        int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
        for (int dir = 0; dir < 8; dir++)
        {
            int x = posx + step[dir][0];
            int y = posy + step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
            {
                continue;
            }
            if (tmp[x][y] != 'o')
            {
                continue;
            }
            int flag = 0;
            while (flag < 15)
            {
                x += step[dir][0];
                y += step[dir][1];
                if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt ||
                    (tmp[x][y] >= '1' && tmp[x][y] <= '9'))
                {
                    break;
                }
                if (tmp[x][y] == 'O')
                {
                    x = posx + step[dir][0];
                    y = posy + step[dir][1];
                    while (tmp[x][y] != 'O')
                    {
                        tmp[x][y] = 'O';
                        x += step[dir][0];
                        y += step[dir][1];
                    }
                }
                flag++;
            }
        }
        for (int i = 0; i < player->row_cnt; i++)
        {
            for (int j = 0; j < player->col_cnt; j++)
            {
                if (tmp[i][j] == 'O')
                {
                    count = count + value[i][j];
                    struct Point ptmp;
                    ptmp.X = i;
                    ptmp.Y = j;
                    count = count + check(player, tmp, ptmp, num + 1);
                }
                else if (tmp[i][j] == 'o')
                {
                    count = count - value[i][j];
                    struct Point ptmp;
                    ptmp.X = i;
                    ptmp.Y = j;
                    count = count + check(player, tmp, ptmp, num + 1);
                }
            }
        }

        for (int i = 0; i < player->row_cnt; i++)
        {
            free(tmp[i]);
        }
        free(tmp);
        return count;
    }
    else if (num == 1)
    { //////////////
        int count = 0;
        int posx = point.X;
        int posy = point.Y;
        char **tmp = (char **)malloc(player->row_cnt * sizeof(char *));
        for (int i = 0; i < player->row_cnt; i++)
        {
            tmp[i] = (char *)malloc(player->col_cnt * sizeof(char));
        }
        for (int i = 0; i < player->row_cnt; i++)
        {
            for (int j = 0; j < player->col_cnt; j++)
            {
                tmp[i][j] = s[i][j];
            }
        }
        tmp[posx][posy] = 'o';
        int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
        for (int dir = 0; dir < 8; dir++)
        {
            int x = posx + step[dir][0];
            int y = posy + step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
            {
                continue;
            }
            if (tmp[x][y] != 'O')
            {
                continue;
            }
            int flag = 0;
            while (flag < 15)
            {
                x += step[dir][0];
                y += step[dir][1];
                if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt ||
                    (tmp[x][y] >= '1' && tmp[x][y] <= '9'))
                {
                    break;
                }
                if (tmp[x][y] == 'o')
                {
                    x = posx + step[dir][0];
                    y = posy + step[dir][1];
                    while (tmp[x][y] != 'o')
                    {
                        tmp[x][y] = 'o';
                        x += step[dir][0];
                        y += step[dir][1];
                    }
                }
                flag++;
            }
        }
        for (int i = 0; i < player->row_cnt; i++)
        {
            for (int j = 0; j < player->col_cnt; j++)
            {
                if (tmp[i][j] == 'o')
                {
                    count = count + value[i][j];
                }
                else if (tmp[i][j] == 'O')
                {
                    count = count - value[i][j];
                }
            }
        }

        for (int i = 0; i < player->row_cnt; i++)
        {
            free(tmp[i]);
        }
        free(tmp);
        return count;
    }
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
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            if (is_valid(player, i, j, my_piece))
            {
                int x = i, y = j;
                char **tmp = (char **)malloc(player->row_cnt * sizeof(char *));
                for (int i = 0; i < player->row_cnt; i++)
                {
                    tmp[i] = (char *)malloc((player->col_cnt + 1) * sizeof(char));
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        tmp[i][j] = player->mat[i][j];
                    }
                    tmp[i][player->col_cnt] = '\0';
                }
                add(x, y, player, my_piece);
                int h1 = -(dfs(player, maxdepth - 1, op_piece, -maxh));
                if (h1 >= maxh)
                {
                    maxh = h1;
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        player->mat[i][j] = tmp[i][j];
                    }
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    free(tmp[i]);
                }
                free(tmp);
                if (maxh >= beta)
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
                char **tmp = (char **)malloc(player->row_cnt * sizeof(char *));
                for (int i = 0; i < player->row_cnt; i++)
                {
                    tmp[i] = (char *)malloc((player->col_cnt + 1) * sizeof(char));
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        tmp[i][j] = player->mat[i][j];
                    }
                    tmp[i][player->col_cnt] = '\0';
                }
                add(x, y, player, 'O');
                int h1 = -dfs(player, 4, 'o', -maxh);
                if (h1 >= maxh)
                {
                    maxh = h1;
                    maxx = x;
                    maxy = y;
                    printf("\n");
                    for (int i = 0; i < player->row_cnt; i++)
                    {
                        printf("%s\n", player->mat[i]);
                    }
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    for (int j = 0; j < player->col_cnt; j++)
                    {
                        player->mat[i][j] = tmp[i][j];
                    }
                }
                for (int i = 0; i < player->row_cnt; i++)
                {
                    free(tmp[i]);
                }
                free(tmp);
            }
        }
    }
    return {maxx, maxy};
}

void pprint(struct Point p)
{
    printf("(%d,%d)\n", p.X + 1, p.Y + 1);
}

void add1(struct Point p, struct Player *player)
{
    int posx = p.X;
    int posy = p.Y;
    player->mat[posx][posy] = 'O';
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
        {
            continue;
        }
        if (player->mat[x][y] != 'o')
        {
            continue;
        }
        int flag = 0;
        while (flag < 15)
        {
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt ||
                (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
            {
                break;
            }
            if (player->mat[x][y] == 'O')
            {
                x = posx + step[dir][0];
                y = posy + step[dir][1];
                while (player->mat[x][y] != 'O')
                {
                    player->mat[x][y] = 'O';
                    x += step[dir][0];
                    y += step[dir][1];
                }
            }
            flag++;
        }
    }
}

void add2(struct Point p, struct Player *player)
{
    int posx = p.X;
    int posy = p.Y;
    player->mat[posx][posy] = 'o';
    int step[8][2] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int dir = 0; dir < 8; dir++)
    {
        int x = posx + step[dir][0];
        int y = posy + step[dir][1];
        if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt)
        {
            continue;
        }
        if (player->mat[x][y] != 'O')
        {
            continue;
        }
        int count = 0;
        while (count < 20)
        {
            count++;
            x += step[dir][0];
            y += step[dir][1];
            if (x < 0 || x >= player->row_cnt || y < 0 || y >= player->col_cnt ||
                (player->mat[x][y] >= '1' && player->mat[x][y] <= '9'))
            {
                break;
            }
            if (player->mat[x][y] == 'o')
            {
                x = posx + step[dir][0];
                y = posy + step[dir][1];
                while (player->mat[x][y] != 'o')
                {
                    player->mat[x][y] = 'o';
                    x += step[dir][0];
                    y += step[dir][1];
                }
            }
        }
    }
}

int main()
{
    struct Player *player;
    player = (Player *)malloc(sizeof(Player));
    player->row_cnt = 8;
    player->col_cnt = 8;
    player->mat = (char **)malloc(10 * sizeof(char *));
    char *tmp;

    for (int i = 0; i < 10; i++)
    {
        tmp = (char *)malloc(10 * sizeof(char));
        player->mat[i] = tmp;
    }
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            player->mat[i][j] = start[i][j];
        }
    }
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            printf("%c", player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    init(player);
o:
    struct Point P = place(player);
    add1(P, player);
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            printf("%c", player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pprint(P);
    struct Point p1;
    scanf("%d %d", &p1.X, &p1.Y);
    p1.X -= 1;
    p1.Y -= 1;
    add2(p1, player);
    for (int i = 0; i < player->row_cnt; i++)
    {
        for (int j = 0; j < player->col_cnt; j++)
        {
            printf("%c", player->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    goto o;
    return 0;
}
