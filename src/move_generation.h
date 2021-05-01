#include <vector>
#include <bitset>
#include <iostream>

typedef std::bitset<64> Bitboard;
using std::cout;
using std::vector;

const int pKing = 0;
const int pQueen = 1;
const int pRook = 2;
const int pKnight = 3;
const int pBishop = 4;
const int pPawn = 5;

const int REGULAR_MOVE = 0;
const int PAWN_EAT = 1;
const int QUEEN_DIAGNOL = 2;
const int QUEEN_STRAIGHT = 3;

vector<Move> generate_moves(Board board, bool White);
int get_row(int index);
bool in_board(int row);