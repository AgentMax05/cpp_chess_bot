#include <string>
#include <vector>
#include <bitset>

typedef std::bitset<64> Bitboard;
using std::string;

class Board {

    public:

    Bitboard pawnW, rookW, bishopW, knightW, queenW, kingW;
    Bitboard pawnB, rookB, bishopB, knightB, queenB, kingB;
    Bitboard boardW, boardB;
    Bitboard attackW, attackB;

    void init_board(string FEN);
    void update_boards();

};

struct Move {
    int piece;
    int type = 0;
    Bitboard move;
    Bitboard legal_check;
};
