#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#ifdef _WIN32
#include "windows_getopt.h"
#else
#include <unistd.h>
#endif
#include <cstring>

#include "board.h"
#include "move_generation.h"
#include "minimax.h"
#include "check_legal.h"
#include "eval.h"
#include "set_attacking.h"
#include "BoardRenderer.h"

typedef std::bitset<64> Bitboard;
using std::cout;
using std::cin;
using std::string;

const int DEFAULT_SEARCH_DEPTH = 8;
const int MAX_SEARCH_DEPTH = 8;

void display_board(Bitboard board);

typedef struct {
    BoardRenderer* renderer;
    bool showHelp;
    bool useWhite;
    string fen;
    int depth;
} ProgramOptions;

void showHelp(char* programName) {
    cout << programName << " [options]\n";
    cout << "OPTIONS\n" 
        << "\t-h                   Shows help message\n" 
        << "\t-f <default|FEN>     Enter a board layout in FEN-notation. Use default for the initial board layout.\n"
        << "\t-b <unicode|ascii>   Force a board representation. Otherwise this is determined by OS.\n"
        << "\t-c <white|black>     Choose the color. Defaults to white.\n"
        << "\t-d <depth>           An integer search depth to use to find moves. Defaults to " << DEFAULT_SEARCH_DEPTH << ".\n"
        << "\n";
}

ProgramOptions getProgramOptions(int argc, char** argv) {
    ProgramOptions options;
    options.renderer = BoardRenderer::getDefaultRenderer();
    options.showHelp = false;
    options.useWhite = true;
    options.depth = DEFAULT_SEARCH_DEPTH;
    options.fen = "";
    
    for(;;)
    {
      switch(getopt(argc, argv, "f:b:hc:d:")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
      {
        case 'f':
          options.fen = string(optarg);
          continue;
        case 'd':
          options.depth = strtol(optarg, NULL, 10);
          if(options.depth == 0 && errno != 0) {
              cerr << "There was an error with the specified depth " << optarg << ". Default will be used instead.\n";
              options.depth = DEFAULT_SEARCH_DEPTH;
          }
          else if(options.depth > MAX_SEARCH_DEPTH) {
              cerr << "The specified depth is larger than the maximum search depth " << MAX_SEARCH_DEPTH << ". The default will be used instead.\n";
              options.depth = DEFAULT_SEARCH_DEPTH;
          }
          continue;
        case 'c':
          if(strcmp(optarg, "white") == 0) {
              options.useWhite = true;
          }
          else if(strcmp(optarg, "black") == 0) {
              options.useWhite = false;
          }
          else {
              cerr << "Invalid color chosen " << optarg << "\n";
              exit(1);
          }
          continue;
        case 'b':
          if(strcmp(optarg, "ascii") == 0) {
              options.renderer = new AsciiBoardRenderer();
          }
          else if(strcmp(optarg, "unicode") == 0) {
              options.renderer = new UnicodeBoardRenderer();
          }
          continue;

        case '?':
        case 'h':
        default :
          options.showHelp = true;
          break;

        case -1:
          break;
      }

      break;
    }
    
    return options;
}


int main(int argc, char** argv) {
    ProgramOptions options = getProgramOptions(argc, argv);
    if(options.showHelp) {
        showHelp(argv[0]);
        return 1;
    }
    
//    BoardRenderer* renderer = BoardRenderer::getDefaultRenderer();
    BoardRenderer* renderer = options.renderer;
    string fen_notation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    string input;
    
    if(options.fen.compare("") == 0) {
        cout << "Enter FEN notation ([default] for default): ";
        cin >> input;
    }
    else if(options.fen.compare("default") == 0) {
        input = "default";
    }
    else {
        input = options.fen;
    }

    if (input != "default") {fen_notation = input;}

    // initialize board object and set attackW and attackB boards
    Board board;
    init_eval_center();
    board.init_board(fen_notation);
    set_attacking(board);

    // display the representation of the board we loaded
    std::cerr << renderer->render(&board) << "\n";

    // search for best move with given depth
    Move move = best_move(board, true, options.depth);
    display_board(move.move);
    
    // cout << "\n" << move.piece;
    
    // now make that move so we can display the updated board
    board.make_move(move.piece, move.move, true);

    std::cerr << "\n" << renderer->render(&board) << "\n";

    // std::vector<Move> moves = generate_moves(board, true);
    // filter_moves(board, moves, true);

    // for (int i = 0; i < moves.size(); i++) {
    //     display_board(moves[i].move);
    //     std::cout << "\n-------------------------\n";
    // }
    // cout << moves.size();    
}

