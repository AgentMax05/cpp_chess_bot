#include "board.h"
#include "move_generation.h"

void set_attacking(Board &board, Move move, bool White) {
   Bitboard pawn, rook, knight, bishop, queen, king, attacking;
   
    int piece = move.piece;

    if (White) {
        pawn =  board.pawnW;
        rook = board.rookW;
        knight = board.knightW;
        bishop = board.bishopW;
        queen = board.queenW;
        king = board.kingW;
        attacking = board.attackW;
    } else {
        pawn = board.pawnB;
        rook = board.rookB;
        knight = board.knightB;
        bishop = board.bishopB;
        queen = board.queenB;
        king = board.kingB;
        attacking = board.attackB;
    }

    Bitboard attacking;
    
    if (piece == pPawn) {pawn = pawn ^ move.move;}
    else if (piece == pRook) {rook = rook ^ move.move;}
    else if (piece == pBishop) {bishop = bishop ^ move.move;}
    else if (piece == pKnight) {knight = knight ^ move.move;}
    else if (piece == pQueen) {queen = queen ^ move.move;}
    else if (piece == pKing) {king = king ^ move.move;}

    for (int i = 0; i < pawn.size(); i++) {
        int current = pawn[i];
        if (current != 1) {continue;}
    }

}

bool check_legal(Board board, Move move, bool White) {
    Bitboard friendly_board, enemy_board;
    if (White) {
        friendly_board = board.boardW;
        enemy_board = board.boardB;
    } else {
        friendly_board = board.boardB;
        enemy_board = board.boardW;
    }

    // check for moving to occupied square
    Bitboard overlap = friendly_board & move.move;
    if (overlap.count() != 1) {return false;}

    // check pawn moves
    if (move.piece == pPawn) {

        // check if taking is legal
        if (move.type == PAWN_EAT) {
            if ((enemy_board & move.move).count() != 1) {return false;}
        }
    }

    // no need to check knight moves


    // check rook moves
    if (move.piece == pRook) {
        Bitboard sliding_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!sliding_legal.none()) {return false;}
    }

    // check bishop moves
    if (move.piece == pBishop) {
        Bitboard diagnol_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!diagnol_legal.none()) {return false;}
    }

    if (move.piece == pQueen) {
        Bitboard queen_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!queen_legal.none()) {return false;}
    }

    set_attacking(board, move, White == false);

}