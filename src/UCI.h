#include <vector>
#include <string>
#include "board.h"

typedef struct {
    char file;
    int rank;
} PGNPosition;

typedef struct {
    int piece;
    PGNPosition from;
    PGNPosition to;
} PGNMove;

typedef struct {
    string fen;
    std::vector<PGNMove> moves;
} Position;

class UCI {
    public:
    void run();
    std::vector<std::string> run_command_position(Position position);
    std::vector<std::string> run_command(std::string line);
    int pgn_position_to_bitboard_index(PGNPosition position);
    Move pgn_move_to_move(PGNMove move);
    private:
    bool started = false;
    bool quit = false;
    bool debug = false;
    bool new_game = false;
    Board board;
    Position position_from_line(string line);
};