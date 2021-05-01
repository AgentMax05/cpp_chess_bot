#include "board.h"
#include "move_generation.h"
#include "eval.h"
#include "minimax.h"

double minimax(Board board, Move move, bool White, int depth) {
    // make move:
    board.make_move(move.piece, move.move, White);

    if (depth == 0) {
        return value_board(board);
    }

    vector<Move> moves = generate_moves(board, White == false);
    
    if (White) {
        double highest = -999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1);
            if (score > highest) {
                highest = score;
                best_move = moves[i];
            }
        }
        return highest;
    } else {
        double lowest = 999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1);
            if (score < lowest) {
                lowest = score;
                best_move = moves[i];
            }
        }
        return lowest;
    }
}


Bitboard best_move(Board board, bool White, int depth) {
    vector<Move> moves = generate_moves(board, White);
    
    if (White) {
        double highest = -999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], false, depth - 1);
            if (score > highest) {
                highest = score;
                best_move = moves[i];
            }
        }

        return best_move.move;

    } else {
        double lowest = 999999;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], true, depth - 1);
            if (score < lowest) {
                lowest = score;
                best_move = moves[i];
            }
        }
        
        return best_move.move;
    }
}