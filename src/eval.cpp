#include "board.h"

double black = -1;
double white = 1;

double pawn = 1;
double rook = 5;
double knight = 3;
double bishop = 3;
double queen = 9;

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

    return white + (-1 * black);

}


double value_board(Board board) {
    double score;
    score += piece_value(board);
    return score;
}