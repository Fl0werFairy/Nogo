#pragma once
#include"ChessBoard.h"
bool be_end(int Player, ChessBoard *CB);
bool judge_available(int x, int y, int Player, ChessBoard *CB);

int Decision(ChessBoard *CB,int degree);