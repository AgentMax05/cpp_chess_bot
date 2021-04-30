#include "board.h"
#include "move_generation.h"
#include "set_attacking.h"

void set_attacking(Board &board, Move move, bool White) {
   Bitboard pawn, rook, knight, bishop, queen, king, attacking;
   int pawn_factor;

    int piece = move.piece;

    if (White) {
        pawn =  board.pawnW;
        rook = board.rookW;
        knight = board.knightW;
        bishop = board.bishopW;
        queen = board.queenW;
        king = board.kingW;
        attacking = board.attackW;
        pawn_factor = -8;
    } else {
        pawn = board.pawnB;
        rook = board.rookB;
        knight = board.knightB;
        bishop = board.bishopB;
        queen = board.queenB;
        king = board.kingB;
        attacking = board.attackB;
        pawn_factor = 8;
    }

    if (piece == pPawn) {pawn = pawn ^ move.move;}
    else if (piece == pRook) {rook = rook ^ move.move;}
    else if (piece == pBishop) {bishop = bishop ^ move.move;}
    else if (piece == pKnight) {knight = knight ^ move.move;}
    else if (piece == pQueen) {queen = queen ^ move.move;}
    else if (piece == pKing) {king = king ^ move.move;}

    // set attacking for pawns
    for (int i = 0; i < pawn.size(); i++) {
        int current = pawn[i];
        if (current != 1) {continue;}
        int row = get_row(i);
        bool space_left = get_row(i - 1) == row;
        bool space_right = get_row(i + 1) == row;

        if (space_left) {attacking[(i + pawn_factor) - 1] = 1;}
        if (space_right) {attacking[(i + pawn_factor) + 1] = 1;}
    }

    // set attacking for knights
    for (int i = 0; i < knight.size(); i++) {
        int current = knight[i];
        if (current != 1) {continue;}
        int row = get_row(i);
    }

}