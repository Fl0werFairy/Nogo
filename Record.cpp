#include"Record.h"
#include"ChessBoard.h"
#include<fstream>

using namespace std;
int cnt_of_records;
void read_records(ChessBoard* arr)
{
    ifstream fin("Records.txt");
    fin >> cnt_of_records;
    for (int k{ 0 }; k < cnt_of_records; k++)
        for (int i{0}; i < 9; i++)
            for (int j{0}; j < 9; j++)fin >> arr[k].board[i][j];
    fin.close();
}
void show_all_records(ChessBoard* arr)
{
    cout << "一共有" << cnt_of_records << "条记录" << endl;
    for (int i{0}; i < cnt_of_records; i++)
    {
        cout << "Num " << i + 1 << ":\n";
        arr[i].output_to_screen();
        cout << endl;
    }
}
void add_record(ChessBoard *arr,ChessBoard *CB)
{
    arr[cnt_of_records++] = *CB;
}
void replace_record(ChessBoard* arr, int k,ChessBoard *CB)
{
    arr[k] = *CB;
}
void write_record(ChessBoard* arr,int t, ChessBoard* CB)
{
    if (t == cnt_of_records+1)add_record(arr, CB);
    else replace_record(arr, t-1, CB);
}
void update_records(ChessBoard* arr)
{
    ofstream fout("Records.txt");
    fout << cnt_of_records << endl;
    for (int i{ 0 }; i < cnt_of_records; i++)
        for (int k{ 0 }; k < 9; k++)
        {
            for (int j{0}; j < 9; j++)fout << arr[i].board[k][j] << ' ';
            fout << endl;
        }
}