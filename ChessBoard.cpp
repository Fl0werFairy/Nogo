#include<iostream>
#include"ChessBoard.h"
#include"Print.h"
#include"config.h"
using namespace std;
extern int const BLACK, WHITE;
void ChessBoard::output_to_screen()
{
    cout << "\033[30m\033[47m   0 1 2 3 4 5 6 7 8 \033[0m" << endl;
        for (int i{0}; i < 9; i++)
        {
            cout <<"\033[30m\033[47m"<< i<<" \033[0m";
            for (int j{0}; j < 9; j++)
            {
                switch (board[i][j])
                {
                case 0:
                    print_board(i, j);
                    break;
                case WHITE:
                    if(j==8)cout << "\033[97m\033[47m● \033[0m";
                    else cout << "\033[97m\033[47m●\033[0m";//显示为白
                    break;
                case BLACK:
                    if (j == 8)cout << "\033[30m\033[47m● \033[0m";
                    else cout << "\033[30m\033[47m●\033[0m";//显示为黑
                    break;
                }
            }
            cout << endl;
        }
}
