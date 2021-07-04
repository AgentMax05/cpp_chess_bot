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
    std::vector<std::string> run_command(std::string line);
    private:
    bool started = false;
    bool quit = false;
    bool debug = false;
    bool new_game = false;
    Board board;
    Position position_from_line(string line);
    std::vector<std::string> run_command_position(Position position);
    Move pgn_move_to_move(PGNMove move);
};