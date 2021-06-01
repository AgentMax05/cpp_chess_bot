#include <vector>
#include <bitset>

typedef std::bitset<64> Bitboard;
using std::vector;

const int pKing = 0;
const int pQueen = 1;
const int pRook = 2;
const int pKnight = 3;
const int pBishop = 4;
const int pPawn = 5;
const int pNone = 6;

const int REGULAR_MOVE = 0;
const int PAWN_EAT = 1;
const int PAWN_DOUBLE = 4;
const int PAWN_ENPASSANT = 7;
const int QUEEN_DIAGNOL = 2;
const int QUEEN_STRAIGHT = 3;
const int L_CASTLE = 5;
const int R_CASTLE = 6;

vector<Move> generate_moves(Board board, bool White);
int get_row(int index);
bool in_board(int row);