#pragma once
#include"ChessBoard.h"
#include<iostream>

extern int cnt_of_records;
void read_records(ChessBoard* arr);
void show_all_records(ChessBoard* arr);
void write_record(ChessBoard* arr, int t, ChessBoard* CB);
void update_records(ChessBoard* arr);