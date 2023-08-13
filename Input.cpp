#include<iostream>
#include"Print.h"
#include"Record.h"
#include"ChessBoard.h"
#include"Play.h"
using namespace std;
extern int cnt_of_records;
int input_choice(int Min,int Max,bool flag=1)
{
    int n;
    while (1)
    {
        cin >> n;
        cout << "\033c";
        //cout<<"\033[H\033[2J";
        if (cin.eof() || cin.bad())
        {
            // 输入为 EOF，或者出现致命错误时，程序终止
            exit(0);
        }
        else if (cin.fail()||(n>Max||n<Min))
        {
            if (flag && n == -3)
            {
                //cout<<"\033[H\033[2J";
                
                return -3;
            }
            // 输入失败（输入的不是整数），清空状态并重新输入
            cout << "输入不合法，请重新输入"<<Min<<"~"<<Max << "之间的整数" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if(n<=Max&&n>=Min)
        {
            return n;
            break;
        }
        
    }
}
bool input_position(int *p,bool f=1)
{
    int a{ 0 };
    a = input_choice(0,8,f);
    if (a < 0)return 0;
    *p = a;
    return 1;
}
bool judge_operations_main_menu(int c,ChessBoard* arr)
{
    int ch{ 0 };
    ChessBoard S;
    switch (c)
    {
    case 1:
        cout << "开始新的对局\n";
        print_instructions();
        Play(&S,arr);
        return 1;
    case 2:
        read_records(arr);
        show_all_records(arr);
        if (cnt_of_records)
        {
            cout << "请输入你选择的存档编号:" << endl;
            ch = input_choice(1, cnt_of_records, 0);
            S = arr[ch - 1];
            cout << "将从当前局面继续对局\n";
            print_instructions();
            Play(&S, arr);
            return 1;
        }
        else
        {
            cout << "当前不存在存档，回到主菜单" << endl;
            print_main_menu();
            return 0;
        }
    default:
        return 1;
    }
}