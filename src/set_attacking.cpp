#include "board.h"
#include "move_generation.h"
#include "set_attacking.h"

// sets attackW and attackB boards
void set_attacking(Board &board) {
    Bitboard pawn, rook, knight, bishop, queen, king, attacking, full_board, friendly_board, enemy_board;
    int pawn_factor;
    full_board = board.complete_board;

    board.attackW.reset();
    board.attackB.reset();

    // cycles through 0 and 1, 0 being black (false), 1 being white (true)
    for (int i = 0; i < 2; i++) {
        bool White = i;

        if (White) {
            pawn =  board.pawnW;
            rook = board.rookW;
            knight = board.knightW;
            bishop = board.bishopW;
            queen = board.queenW;
            king = board.kingW;
            attacking = board.attackW;
            pawn_factor = -8;

            friendly_board = board.boardW;
            enemy_board = board.boardB;
        }

        else {
            pawn = board.pawnB;
            rook = board.rookB;
            knight = board.knightB;
            bishop = board.bishopB;
            queen = board.queenB;
            king = board.kingB;
            attacking = board.attackB;
            pawn_factor = 8;

            friendly_board = board.boardB;
            enemy_board = board.boardW;
        }


        // set attacking for pawns
        for (int i = 0; i < pawn.size(); i++) {
            if (pawn[i] != 1) {continue;}
            int row = get_row(i);
            bool space_left = get_row(i - 1) == row;
            bool space_right = get_row(i + 1) == row;
            bool space_move = in_board(get_row(i + pawn_factor));


            if (space_left && space_move) {attacking[(i + pawn_factor) - 1] = 1;}
            if (space_right && space_move) {attacking[(i + pawn_factor) + 1] = 1;}
        }

        // set attacking for knights
        for (int i = 0; i < knight.size(); i++) {
            if (knight[i] != 1) {continue;}
            int row = get_row(i);

            bool space_above = in_board(get_row(i - 8));
            bool double_above = in_board(get_row(i - 16));

            bool space_below = in_board(get_row(i + 8));
            bool double_below = in_board(get_row(i + 16));

            bool space_left = get_row(i - 1) == row;
            bool double_left = get_row(i - 2) == row;

            bool space_right = get_row(i + 1) == row;
            bool double_right = get_row(i + 2) == row;

            // up left
            if (double_above && space_left) {attacking[i - 16 - 1] = 1;}
            // up right
            if (double_above && space_right) {attacking[i - 16 + 1] = 1;}
            // down left
            if (double_below && space_left) {attacking[i + 16 - 1] = 1;}
            // down right
            if (double_below && space_right) {attacking[i + 16 + 1] = 1;}
            // left up
            if (double_left && space_above) {attacking[i - 2 - 8] = 1;}
            // left down
            if (double_left && space_below) {attacking[i - 2 + 8] = 1;}
            // right up
            if (double_right && space_above) {attacking[i + 2 - 8] = 1;}
            // right down
            if (double_right && space_below) {attacking[i + 2 + 8] = 1;}

        }

        // set attacking for rooks
        for (int i = 0; i < rook.size(); i++) {
            if (rook[i] != 1) {continue;}
            int row = get_row(i);

            // horizontal right
            int x = i + 1;
            while (get_row(x) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x++;
            }

            // horizontal left
            x = i - 1;
            while (get_row(x) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x--;
            }

            // up
            x = i - 8;
            while (in_board(get_row(x))) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x -= 8;
            }

            // down
            x = i + 8;
            while (in_board(get_row(x))) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x += 8;
            }
        }

        // set attacking for bishops
        for (int i = 0; i < bishop.size(); i++) {
            if (bishop[i] != 1) {continue;}
            int row = get_row(i);

            // up right diagnol
            int x = i - 8 + 1;
            int x_value = i + 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x - 8 + 1;
                x_value += 1;
            }

            // up left diagnol
            x = i - 8 - 1;
            x_value = i - 1;
            while (in_board(get_row(x)) && get_row(x_value) ==  row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x - 8 - 1;
                x_value--;
            }

            // down right diagnol
            x = i + 8 + 1;
            x_value = i + 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x + 8 + 1;
                x_value++;
            }

            // down left diagnol
            x = i + 8 - 1;
            x_value = i - 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x + 8 - 1;
                x_value--;
            }
        }

        // set attacking for queens
        for (int i = 0; i < queen.size(); i++) {
            if (queen[i] != 1) {continue;}
            int row = get_row(i);

            // horizontal and vertical (rook moves)

            // horizontal right
            int x = i + 1;
            while (get_row(x) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x++;
            }

            // horizontal left
            x = i - 1;
            while (get_row(x) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x--;
            }

            // vertical up
            x = i - 8;
            while (in_board(get_row(x))) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x -= 8;
            }

            // vertical down
            x = i + 8;
            while (in_board(get_row(x))) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x += 8;
            }

            // diagnol moves (bishop moves)

            // up right diagnol
            x = i - 8 + 1;
            int x_value = i + 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x - 8 + 1;
                x_value += 1;
            }

            // up left diagnol
            x = i - 8 - 1;
            x_value = i - 1;
            while (in_board(get_row(x)) && get_row(x_value) ==  row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x - 8 - 1;
                x_value--;
            }

            // down right diagnol
            x = i + 8 + 1;
            x_value = i + 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x + 8 + 1;
                x_value++;
            }

            // down left diagnol
            x = i + 8 - 1;
            x_value = i - 1;
            while (in_board(get_row(x)) && get_row(x_value) == row) {
                attacking[x] = 1;
                if (full_board[x] == 1) {break;}
                x = x + 8 - 1;
                x_value--;
            }

        }

        // set attacking for king
        for (int i = 0; i < king.size(); i++) {
            if (king[i] != 1) {continue;}

            int row = get_row(i);

            bool space_left = get_row(i - 1) == row;
            bool space_right = get_row(i + 1) == row;
            bool space_above = in_board(row - 1);
            bool space_below = in_board(row + 1);

            // left
            if (space_left) {attacking[i - 1] = 1;}
            // right
            if (space_right) {attacking[i + 1] = 1;}
            // down
            if (space_below) {attacking[i + 8] = 1;}
            // up
            if (space_above) {attacking[i - 8] = 1;}
            // left down
            if (space_left && space_below) {attacking[i - 1 + 8] = 1;}
            // left up
            if (space_left && space_above) {attacking[i - 1 - 8] = 1;}
            // right down
            if (space_right && space_below) {attacking[i + 1 + 8] = 1;}
            // right up
            if (space_right && space_above) {attacking[i + 1 - 8] = 1;}

        }

        if (White) {
            board.attackW = attacking;
        } else {
            board.attackB = attacking;
        }
    }
}