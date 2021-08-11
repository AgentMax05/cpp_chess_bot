#include <iostream>
#include <vector>
#include "UCI.h"
#include "board.h"
#include "set_attacking.h"
#include "move_generation.h"

using namespace std;

void UCI::run() {
    string line;
    while (getline(cin, line)) {
        // TODO: might need to strip line
        run_command(line);
        if(UCI::quit) {
            cout << "quitting!\n";
            return;
        }
    }
    
}

vector<string> UCI::run_command(string line) {
    vector<string> output;

    if (line == "uci") {
        UCI::started = true;
        output.push_back("id name something\n");
        output.push_back("id author Max and Jamie\n");
        output.push_back("uciok");
        return output;
    }
    if (line.rfind("debug") != -1) {
        bool debug = line.rfind("on");
        UCI::debug = debug;
        return output;
    }
    if (line == "quit") {
        UCI::quit = true;
        return output;
    }
    if (line.rfind("setoption") != -1) {
        return output;
    }
    if (line == "isready") {
        // TODO: possibly do move generation
        output.push_back("readyok");
        return output;
    }
    if (line == "ucinewgame") {
        UCI::new_game = true;
        return output;
    }
    
    if (line.rfind("position") != -1) {
        return run_command_position(position_from_line(line));
    }

    cerr << "Unrecognized command" << line << endl;
    return output;
}

Position UCI::position_from_line(string line) {
    // converts the line to a position
    Position pos;
    pos.fen = BoardConstants::DefaultFEN;
    vector<PGNMove> moves;
    // TODO: fill in some moves
    pos.moves = moves;
    return pos;
}

bool is_piece_white(int piece) {
    // TODO: 
    return false;
}

/*
 *  Bitboard Numbering:
 *  RANK | LBIT    -    RBIT
 *    8     0 > 00000000 <7
 *    7     8 > 00000000 <15
 *    6     16> 00000000 <23
 *    5     24> 00000000 <31
 *    4     32> 00000000 <39
 *    3     40> 00000000 <47
 *    2     48> 00000000 <55
 *    1     56> 00000000 <63
 *    FILE      ABCDEFGH
 * 
 *  Right shift (>>) moves pieces up
 *  Left shift (<<) moves pieces down
*/

int UCI::pgn_position_to_bitboard_index(PGNPosition position) {
    int column = position.file - 65; // A-H
    int row = 8 - position.rank;

    return row * 8 + column;
}

Move UCI::pgn_move_to_move(PGNMove pgnMove) {
    Bitboard move_board;
    Bitboard legal_check;
    int piece = pgnMove.piece;
    Move move;
    move.piece = piece;
    move.type = REGULAR_MOVE;
    int to = pgn_position_to_bitboard_index(pgnMove.to);
    int from = pgn_position_to_bitboard_index(pgnMove.from);
    move_board[to] = 1;
    move_board[from] = 1;
    move.move = move_board;
    move.legal_check = legal_check;
    return move;
}

vector<std::string> UCI::run_command_position(Position position) {
    vector<string> output;

    // position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
    Board board;
    board.init_board(position.fen);

    set_attacking(board);
    for(int i=0; i<position.moves.size(); i++) {
        Move move = pgn_move_to_move(position.moves[i]);
        board.make_move(move.piece, move.move, is_piece_white(move.piece));
    }

    return output;
    
    // // search for best move with given depth
    // Move move = best_move(board, options.useWhite, options.depth);
    // display_board(move.move);
    
    // // cout << "\n" << move.piece;
    
    // // now make that move so we can display the updated board
    // board.make_move(move.piece, move.move, options.useWhite);

}
