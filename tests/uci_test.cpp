#include <gtest/gtest.h>
#include <iostream>
#include "UCI.h"
#include <vector>
#include "move_generation.h"
#include "board.h"
using namespace std;

TEST(UCITest, ReturnsUCIOk) {
    UCI uci;
    vector<string> output = uci.run_command("uci");
    string last_line = output[output.size() - 1];
    EXPECT_EQ("uciok", last_line);
}

TEST(UCITest, PgnPositionToBitboardIndex1) {
    UCI uci;
    PGNPosition position = {
        .file = 'E',
        .rank = 2
    };
    int index = uci.pgn_position_to_bitboard_index(position);

    EXPECT_EQ(index, 52);
}

TEST(UCITest, PgnPositionToBitboardIndex2) {
    UCI uci;
    PGNPosition position = {
        .file = 'E',
        .rank = 4
    };
    int index = uci.pgn_position_to_bitboard_index(position);

    EXPECT_EQ(index, 36);
}

TEST(UCITest, PgnMoveToMove) {
    UCI uci;
    PGNMove pgnMove = {
        .piece = pPawn,
        .from = PGNPosition {
            .file = 'E',
            .rank = 2
        },
        .to = PGNPosition {
            .file = 'E',
            .rank = 4
        }
    };
    Move move = uci.pgn_move_to_move(pgnMove);

    EXPECT_EQ(move.piece, pgnMove.piece);
    EXPECT_EQ(move.type, REGULAR_MOVE);
    Bitboard move_board;
    move_board[52] = 1;
    move_board[36] = 1;
    // 00000000
    // 00000000
    // 00000000
    // 00000000
    // 0000P000
    // 00000000
    // 0000P000
    // 00000000
    EXPECT_EQ(move.move, move_board); 
    Bitboard legal_check;
    EXPECT_EQ(move.legal_check, legal_check); 
}

// TEST(UCITest1, RunCommandPosition) {
//     Position position; // fill
//     position.fen = BoardConstants::DefaultFEN;
//     PGNMove move = {
//         .piece = pPawn,
//         .from = PGNPosition {
//             .file = 'E',
//             .rank = 2
//         },
//         .to = PGNPosition {
//             .file = 'E',
//             .rank = 4
//         }
//     };
//     vector<PGNMove> moves;
//     moves.push_back(move);
//     position.moves = moves;
//     UCI uci;
//     vector<string> output = uci.run_command_position(position);
//     string last_line = output[output.size() - 1];
//     // ask UCI for the board position...
//     // test what the board looks like
//     EXPECT_EQ(0, output.size());
// }
