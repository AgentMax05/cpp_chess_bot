#include "board.h"

double black = -1;
double white = 1;

double pawn = 1;
double rook = 5;
double knight = 3;
double bishop = 3;
double queen = 9;

Bitboard center_squares;

double stacked_pawn = -1.5;
double material_factor = 0.8;
double check = 10;
double center = 1;

void init_eval_center() {
    for (int i = 26; i < 30; i++) {
    center_squares[i] = 1;
    }
    for (int i = 34; i < 38; i++) {
        center_squares[i] = 1;
    }
}

double attacked_squares(Board board) {
    double white = board.attackW.count();
    double black = -1 * board.attackB.count();
    return white + black;
}

double king_check(Board board) {
    double white = 0;
    double black = 0;
    
    if ((board.kingW & board.attackB).none() == false) {white += check;}
    if ((board.kingB & board.attackW).none() == false) {black -= check;} 

    return white + black;

}

double center_control(Board board) {
    double white = 0;
    double black = 0;

    white += ((board.attackW & center_squares).count() * center);
    black -= ((board.attackB & center_squares).count() * center);

    return white + black;

}

double stacked_pawns(Board board) {
    double white = 0;
    double black = 0;

    for (int i = 0; i < 56; i++) {
        if (board.pawnW[i] == 1) {
            if (board.pawnW[i + 8] == 1) {white += stacked_pawn;}
        }
        if (board.pawnB[i] == 1) {
            if (board.pawnB[i + 8] == 1) {black -= stacked_pawn;}
        }
    }
    return white + black;
}

double piece_value(Board board) {
    double white = 0;
    double black = 0;

    white += board.pawnW.count() * pawn;
    white += board.rookW.count() * rook;
    white += board.knightW.count() * knight;
    white += board.bishopW.count() * bishop;
    white += board.queenW.count() * queen;

    black += board.pawnB.count() * pawn;
    black += board.rookB.count() * rook;
    black += board.knightB.count() * knight;
    black += board.bishopB.count() * bishop;
    black += board.queenB.count() * queen;

    return (white + (-1 * black)) * material_factor;

}


double value_board(Board board) {
    double score;
    score += piece_value(board);
    score += stacked_pawns(board);
    score += king_check(board);
    score += center_control(board);
    score += attacked_squares(board);

    return score;
}