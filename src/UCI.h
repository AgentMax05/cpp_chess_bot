#include <vector>
#include <string>

class UCI {
    private:
    void run();
    vector<std::string> run_command(std::string line);
    bool started = false;
    bool quit = false;
    bool debug = false;
    bool new_game = false;
    Board board;
    vector<std::string> run_command_position(std::string line);
};