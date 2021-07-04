#include <iostream>
#include <vector>
#include "UCI.h"
#include "board.h"
#include "set_attacking.h"

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

Move UCI::pgn_move_to_move(PGNMove pgnMove) {
    Move move;
    cerr << "TODO: implement pgn_move_to_move" << endl;
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
