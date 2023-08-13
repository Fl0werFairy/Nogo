#include"ChessBoard.h"
#include"Decision.h"
#include"Input.h"
#include"Print.h"
#include"Record.h"
#include<iostream>
#include"config.h"
using namespace std;
bool end_game;
extern int const WHITE, BLACK;
void Play(ChessBoard *CB,ChessBoard *arr)
{
	cout << "请输入想要选择的难度(1~10,数字越大难度越大，相应的AI思考时间会变长)"<<endl;
	int diff = input_choice(1, 10, 0);
	bool surrend{ 0 };
	end_game = 0;
	int x{ 0 }, y{ 0 };
	bool black_lose = be_end(BLACK,CB), white_lose =0;
	while (!black_lose && !white_lose&&!surrend&&!end_game)
	{
		CB->output_to_screen();
		black_lose = be_end(BLACK, CB);
		if (black_lose)break;
		cout << "轮到你落子，请输入坐标" << endl;
		bool call_up_menu =!input_position(&x);
		while (call_up_menu)
		{
			print_menu_halfway();
			int op = input_choice(1, 5, 0);
			int ch{ 0 };
			switch (op)
			{
			case 1:CB->output_to_screen(); break;
			case 4: end_game = 1; return;
			case 5:
				cout << "请输入1~10之间的整数" << endl;
				diff = input_choice(1, 10, 0);
				cout << "已调整难度为" << diff << endl;
				cout << "游戏继续" << endl;
				CB->output_to_screen();
				break;
			case 2:
				read_records(arr);
				show_all_records(arr);
				cout << "你想将该局面存储在几号存档？" << endl;
				if(cnt_of_records<19)cout << "输入" << cnt_of_records + 1 << "代表您想新建存档" << endl;
				if (cnt_of_records > 0 && cnt_of_records < 19)cout << "输入1~" << cnt_of_records << "的整数表示您想覆盖存档" << endl;
				else if (cnt_of_records == 19)cout << "输入1~" << cnt_of_records << "的整数表示您想覆盖存档" << endl;
				ch = input_choice(1, min(cnt_of_records + 1, 19), 0);
				write_record(arr, ch, CB);
				update_records(arr);
				cout << "操作成功！" << endl;
				CB->output_to_screen();
				
				break;
			case 3:
				read_records(arr);
				show_all_records(arr);
				if (!cnt_of_records)
				{
					cout << "当前不存在存档，已返回上级" << endl;
					CB->output_to_screen();
					cout << "请输入落子坐标！" << endl;
				}
				else
				{
					cout << "请输入你想要读取的存档编号" << endl;
					ch = input_choice(1, cnt_of_records, 0);
					*CB = arr[ch - 1];
					Play(CB, arr);
					return;
				}
				break;
			}
			call_up_menu=!input_position(&x);
		}
		input_position(&y,0);
		call_up_menu = 0;
		while (1)
		{
			if (CB->board[x][y] != 0)
			{
				cout << "不可下在已有棋子的地方，请重新输入坐标" << endl;
				CB->output_to_screen();
				input_position(&x);
				input_position(&y, 0);
				continue;
			}
			if (!judge_available(x,y,1,CB))
			{
				cout << "不可以自杀/提子，请重新输入坐标"<<endl;
				CB->output_to_screen();
				input_position(&x);
				input_position(&y, 0);
				continue;
			}
			break;
		}
		CB->board[x][y] = BLACK;
		white_lose = be_end(WHITE, CB);
		if (!white_lose)
		{
			cout << "AI正在思考，请耐心等待"<<diff-1<< "~" << diff << "秒钟......" << endl;
			int pos = Decision(CB,diff);
			int gx{ pos / 9 }, gy{ pos % 9 };
			if (judge_available(gx, gy, WHITE, CB))
			{
				CB->board[gx][gy] = WHITE;
				cout<<"\033c";
				cout << "请注意，AI上一步落子在(" << gx << ' ' << gy << ")处" << endl;
			}
			else surrend = 1;
		}
	}
	if (black_lose)cout << "你输了"<<endl;
	else if (white_lose || surrend)cout << "电脑认输了！恭喜你战胜了AI！"<<endl;
	print_main_menu();
	return;
}