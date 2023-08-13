#include<iostream>
#include<Windows.h>
#include"ChessBoard.h"
#include"Print.h"
#include"Record.h"
#include"Input.h"
#include"Play.h"
using namespace std;
int main()
{
	SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode{};
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(hOut, mode);
    

    int c{ 0 };
    ChessBoard B[20];
    bool succ{ 0 };
    cout << "\033]0;不围棋小游戏（制作者：黄煦喆）\007";
    print_main_menu();
    while (c != 3&&!end_game&&!succ)
    {
        c = input_choice(1, 3, false);
        succ=judge_operations_main_menu(c, B);
    }
    cout << "游戏结束，再见！" << endl;
    update_records(B);
	return 0;
}