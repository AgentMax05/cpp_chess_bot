#include "board.h"
#include "move_generation.h"
#include "eval.h"
#include "minimax.h"
#include "check_legal.h"
#include "set_attacking.h"

int nodes = 0;

double minimax(Board board, Move move, bool White, int depth, double alpha, double beta) {
    nodes++;
    // make move:
    board.make_move(move.piece, move.move, White);
    set_attacking(board);

    if (depth == 0) {
        return value_board(board);
    }

    // generate moves for other side (White == false)
    vector<Move> moves = generate_moves(board, White == false);
    filter_moves(board, moves, White == false);

    if (White) {
        double highest = -999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1, alpha, beta);
            if (score > highest) {
                highest = score;
                best_move = moves[i];
                if (score > alpha) {
                    alpha = score;
                }
            }
            if (beta <= alpha) {
                return highest;
            }
        }
        return highest;
    } else {
        double lowest = 999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1, alpha, beta);
            if (score < lowest) {
                lowest = score;
                best_move = moves[i];
                if (lowest < beta) {
                    beta = lowest;
                }
            }
            if (beta <= alpha) {
                return lowest;
            }
        }
        return lowest;
    }
}


Move best_move(Board board, bool White, int depth) {
    vector<Move> moves = generate_moves(board, White);
    filter_moves(board, moves, White);

    nodes = 0;

    if (White) {
        double highest = -999999;
        Move best;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], true, depth - 1, -999999, 999999);
            if (score > highest) {
                highest = score;
                best = moves[i];
            }
        }
        cout << "\nnodes searched: " << nodes;
        return best;

    } else {
        double lowest = 999999;
        Move best;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], false, depth - 1, -999999, 999999);
            if (score < lowest) {
                lowest = score;
                best = moves[i];
            }
        }
        cout << "\nnodes searched: " << nodes;
        return best;
    }
}