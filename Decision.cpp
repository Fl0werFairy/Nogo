#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include<random>
#include"ChessBoard.h"
#include"pro.h"
#include"Decision.h"
#include <chrono>
#include"config.h"
using namespace std::literals;

using namespace std;

int depth{ 6 }; //模拟深度
const double C{ 2 };  //UCB常数
const double c1{ 1 }, c2{ 0.01 }; //禁入点和虎口的加权值
int Player{ 1 }, Player0;   //当前颜色，1为黑，-1为白 Player0为搜索时储存Player初状态
int Printx{ -1 }, Printy{ -1 }; //落子点的坐标
int Nodex[82], Nodey[82];
int value, times;
int available;
int num; //Node当前下标
int Avail[2][81]; //双方可落子点 1为黑，0为白
int len[2]; //可落子点数量 1为黑，0为白
int temp_avail[2][81]; //保存初状态
int temp_len[2]; //保存初状态
pro Node[1500000] = {}; //节点信息
inline int C_to_N(int color) //将1/-1转为1/0
{
    return (color + 1) / 2;
}
inline int N_to_C(int number) //将1/0转为1/-1
{
    return (number * 2 - 1);
}
inline void Store()
{
    for (int i{ 0 }; i < 2; i++)
        for (int j{ 0 }; j < 81; j++)temp_avail[i][j] = Avail[i][j];
    temp_len[0] = len[0];
    temp_len[1] = len[1];
}
inline void Recover()
{
    for (int i{ 0 }; i < 2; i++)
        for (int j{ 0 }; j < 81; j++)Avail[i][j] = temp_avail[i][j];
    len[0] = temp_len[0];
    len[1] = temp_len[1];
}
bool has_one_qi(int x, int y, int player,ChessBoard *CB) //判断某点是不是虎口
{
    int p = 0, q = 0;
    if (x - 1 < 0 || CB->board[x - 1][y] == Player) p++;
    else if (x - 1 >= 0 && CB->board[x - 1][y] == 0 && q == 0) q++;
    else return 0;
    if (x + 1 > 8 || CB->board[x + 1][y] == Player) p++;
    else if (x + 1 <9 && CB->board[x + 1][y] == 0 && q == 0) q++;
    else return 0;
    if (y - 1 < 0 || CB->board[x][y - 1] == Player) p++;
    else if (y - 1 >= 0 && CB->board[x][y - 1] == 0 && q == 0) q++;
    else return 0;
    if (y + 1 > 8 || CB->board[x][y + 1] == Player) p++;
    else if (y + 1 <9 && CB->board[x][y + 1] == 0 && q == 0) q++;
    else return 0;
    return (p == 3 && q == 1);//网开一面，q是开的那一面，p是被“堵住”的
}
bool DFS(int x, int y, int Player,ChessBoard *CB) //判断点x，y与周围同色子是否有气
{
    if (x == -1 || y == -1 || x == 9 || y == 9) //越界
        return 0;
    if (CB->board[x][y] == -Player) //另一色棋子
        return 0;
    if (CB->board[x][y] == 0) //无子则有气
        return 1;
    CB->board[x][y] = -Player;
    bool b = DFS(x, y - 1, Player,CB) || DFS(x, y + 1, Player,CB)
        || DFS(x - 1, y, Player,CB) || DFS(x + 1, y, Player,CB);
    CB->board[x][y] = Player;
    return b;
}
bool judge_available(int x, int y, int Player,ChessBoard *CB) //判断点x,y能否由player下子
{
    if (x < 0 || y < 0 || x>8 || y>8 || CB->board[x][y] != 0)
        return 0;
    if ((x == 8 || CB->board[x + 1][y] == -Player) && (x == 0 || CB->board[x - 1][y] == -Player)
        && (y == 0 || CB->board[x][y - 1] == -Player) && (y == 8 || CB->board[x][y + 1] == -Player))
        return 0;
    bool b = 1;
    CB->board[x][y] = Player;
    if (!DFS(x, y, Player,CB) //自杀
        || (x + 1 <9 && CB->board[x + 1][y] == -Player && !DFS(x + 1, y, -Player,CB))
        || (x - 1 >= 0 && CB->board[x - 1][y] == -Player && !DFS(x - 1, y, -Player,CB))
        || (y + 1 <9 && CB->board[x][y + 1] == -Player && !DFS(x, y + 1, -Player,CB))
        || (y - 1 >= 0 && CB->board[x][y - 1] == -Player && !DFS(x, y - 1, -Player,CB))) //使对手无气
        b = 0;
    CB->board[x][y] = 0;
    return b;
}
double board_value1(int Player,ChessBoard *CB) //加入虎口数量的棋局价值
{
    double p = 0;
    for (int i{0}; i <9; i++)
        for (int j{0}; j <9; j++)
        {
            if (!judge_available(i, j, -Player,CB))
                p += c1;
            else if (CB->board[i][j] == 0 && has_one_qi(i, j, -Player,CB))
                p -= c2;
            if (!judge_available(i, j, Player,CB))
                p -= c1;
            else if (CB->board[i][j] == 0 && has_one_qi(i, j, Player,CB))
                p += c2;
        }
    return p;
}
void get_avail(int Player,ChessBoard *CB)
{
    if (Player == 0)
    {
        len[1] = len[0] = 0;
        memset(Avail, 0, sizeof(Avail));

        for (int i{0}; i <9; i++)
            for (int j{0}; j <9; j++)
            {
                if (judge_available(i, j, BLACK,CB))
                    Avail[C_to_N(BLACK)][len[C_to_N(BLACK)]++] = i * 9 + j;
                if (judge_available(i, j, WHITE,CB))
                    Avail[C_to_N(WHITE)][len[C_to_N(WHITE)]++] = i * 9 + j;
            }
    }
    else
    {
        int number = C_to_N(Player);
        int index = 0, x, y;
        while (index < len[number])
        {
            x = Avail[number][index] / 9;
            y = Avail[number][index] % 9;
            if (judge_available(x, y, Player,CB))
                index++;
            else
            {
                Avail[number][index] = Avail[number][len[number] - 1];//删去不可以落子的点
                len[number]--;
            }
        }
    }
}
bool be_end(int Player,ChessBoard *CB) //判断是否终局
{
    for (int i{0}; i <9; i++)
        for (int j{0}; j <9; j++)
            if (judge_available(i, j, Player,CB))
                return 0;
    return 1;
}
inline bool be_end1(int Player)
{
    return (len[C_to_N(Player)] == 0);
}
int count_available_steps(int Player,ChessBoard *CB)
{
    int p{ 0 };
    for (int i{0}; i <9; i++)
        for (int j{0}; j <9; j++)
            if (judge_available(i, j, Player,CB))
                p++;
    return p;
}
int minmax(int k,ChessBoard *CB) //搜索，必胜返回1，必败返回0
{//minimax是悲观算法，在矮子里面拔长子
    int n1{ 0 }, n2{ 0 };
    int Points[82];//对方不能下的点位
    for (int i{0}; i <9; i++)
        for (int j{0}; j <9; j++)
        {
            if (judge_available(i, j, Player,CB))
            {
                if (n1 == 0 && k == 1) { Printx = i; Printy = j; }//下这里必胜
                if (judge_available(i, j, -Player,CB))
                {
                    n1++;
                    CB->board[i][j] = Player;
                    Player = -Player;
                    if (minmax(k + 1,CB) == 0)
                    {
                        if (k == 1) { Printx = i; Printy = j; }
                        CB->board[i][j] = 0;
                        Player = -Player;
                        return 1;
                    }
                    CB->board[i][j] = 0;
                    Player = -Player;
                }
                else Points[++n2] = i * 9 + j;
            }
        }
    if (n1 + n2 == 0)
        return 0;
    for (int t{1}; t <= n2; t++)
    {
        int x{ Points[t] / 9 }, y{ Points[t] % 9 };
        CB->board[x][y] = Player;
        Player = -Player;
        if (minmax(k + 1,CB) == 0)
        {
            if (k == 1) { Printx = x; Printy = y; }
            CB->board[x][y] = 0;
            Player = -Player;
            return 1;
        }
        CB->board[x][y] = 0;
        Player = -Player;
    }
    return 0;
}
int UCB1(int now,ChessBoard *CB)
{
    int choose = 0;
    if (!Node[now].Expanded)
    {
        int *pos;
        int number = C_to_N(Player);
        int temp_len, temp_avail[81];
        temp_len = len[number];
        memcpy(temp_avail, Avail[number], sizeof(temp_avail));
        //删除已选择过的点
        for (int i{1}; i <= Node[now].now_children; i++)
        {
            int coordinate = Node[Node[now].Children[i]].x * 9 + Node[Node[now].Children[i]].y;
            pos = find(temp_avail, temp_avail + temp_len, coordinate);//在区间内找第三个参数
            if (pos != temp_avail + temp_len)//上一排中pos找到了
            {
                *pos = temp_avail[temp_len - 1];
                temp_len--;
            }
        }
        while (1)
        {
            if (temp_len == 0 && Node[now].now_children == 0)//终局
                return -1;
            else if (temp_len == 0 && Node[now].now_children != 0)//没有能走赢的
            {
                Node[now].Expanded = 1;
                choose = -1;
                break;
            }
            random_device rd; // 构造真随机数生成引擎
            auto seed{ rd() };       // 产出一个随机数作为种子

            // 用 seed 作为种子构造伪随机数生成器
            default_random_engine e(seed);
            uniform_int_distribution<> d(0, temp_len-1);
            int select = d(e) ;


            int sx = temp_avail[select] / 9;
            int sy = temp_avail[select] % 9;
            if (judge_available(sx, sy, Player,CB))
            {
                pos = find(Avail[1 - number], Avail[1 - number] + len[1 - number], temp_avail[select]);
                if (pos != Avail[1 - number] + len[1 - number])
                {
                    *pos = Avail[1 - number][len[1 - number] - 1];
                    len[1 - number]--;
                }
                choose = temp_avail[select];
                break;
            }
            else
            {
                pos = find(Avail[number], Avail[number] + len[number], temp_avail[select]);
                *pos = Avail[number][len[number] - 1];
                len[number]--;
                temp_avail[select] = temp_avail[temp_len - 1];
                temp_len--;
            }
        }
        if (choose != -1)
        {
            Node[now].now_children++;
            Node[now].Children[Node[now].now_children] = ++num;
            Node[num] = { choose / 9, choose % 9 };
            pos = find(Avail[number], Avail[number] + len[number], choose);
            *pos = Avail[number][len[number] - 1];
            len[number]--;
            return num;
        }
    }
    //完全拓展用ucb选择
    double max = -10000;
    for (int i{1}; i <= Node[now].now_children; i++)
    {
        int child = Node[now].Children[i];
        double value = Node[child].Value / Node[child].Times + C * sqrt(log(Node[now].Times) / Node[child].Times);
        if (value > max)
        {
            max = value;
            choose = child;
        }
    }
    return choose;
}
void UCT(ChessBoard *CB,int degree=2)//ucb+mct
{
    int search_node[100]; //记录搜索路径
    num = 0;
    Node[0] = { -1, -1 };
    get_avail(0,CB);
    Store();
    chrono::duration<double> timeout=1s;
    timeout = timeout * degree - 0.4s;
    auto p1{ chrono::system_clock::now() };
    auto p2{ chrono::system_clock::now() };
    
    while (p2 - p1 < timeout)
    {
        int i{ 0 };
        int ucb{ 0 };
        int initial = UCB1(ucb,CB);
        for (int t{1}; t <= 10; t++)//10次探索，UCB1自带随机性
        {
            ucb = initial;
            i = 1;
            search_node[i] = ucb;
            CB->board[Node[ucb].x][Node[ucb].y] = Player;
            Player = -Player;
            while (!be_end1(Player) && i <= depth)
            {
                ucb = UCB1(ucb,CB);
                if (ucb == -1)break;
                i++;
                search_node[i] = ucb;
                CB->board[Node[ucb].x][Node[ucb].y] = Player;
                Player = -Player;
            }
            int pl = 1;
            double final_value = board_value1(-Player,CB);
            for (int j{i}; j > 0; j--)
            {
                ++Node[search_node[j]].Times;
                Node[search_node[j]].Value += final_value;
                
                int bb = !judge_available(Node[search_node[j]].x, Node[search_node[j]].y,Player,CB);
                Player = -Player;
                if (bb)
                {
                    if (pl == 1)Node[search_node[j]].Value -= 5;
                    else Node[search_node[j]].Value += 5;
                }
                pl=-pl;
                final_value = -final_value;
                CB->board[Node[search_node[j]].x][Node[search_node[j]].y] = 0;
            }
            Node[0].Times++;
            Player = Player0;
            Recover();
        }
        p2 = chrono::system_clock::now();
    }
    double max = -10000;
    int max_node = 0;
    for (int i{1}; i <= Node[0].now_children; i++)
    {
        int children{ Node[0].Children[i] };
        if (Node[children].Times != 0)
        {
            double children_value = Node[children].Value / Node[children].Times;
            if (children_value > max&&judge_available(Node[children].x,Node[children].y,-1,CB))
            {
                max = children_value;
                max_node = children;
            }
        }
    }
    value = Node[max_node].Value;
    times = Node[max_node].Times;
    Printx = Node[max_node].x;
    Printy = Node[max_node].y;
    Player = Player0;
}
void evaluate(ChessBoard *CB,int degree)
{
    Player0 = Player;
    available = 0;
    for (int i{0}; i <9; i++)
        for (int j{0}; j <9; j++)
            available += judge_available(i, j, Player,CB);
    if (available > 49)
        depth = 6;
    else if (available > 36)
        depth = 8;
    else if (available > 20)
        depth = 10;
    else
        depth = 99;
    if (available > 10)
        UCT(CB,degree);
    else
        minmax(1,CB);
}
int Decision(ChessBoard *CB,int degree)
{
    evaluate(CB,degree);
    return 9*Printx+Printy;
}