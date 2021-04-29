#include <string>
#include <bitset>
#include <iostream>

using std::string;

bool is_lowercase(char input) {
    int char_int = int(input);

    if (char_int >= 97 && char_int <= 122) {
        return true;
    }
    return false;
}

bool is_num(char input) {
    int char_int = int(input);

    if (char_int >= 48 && char_int <= 57) {
        return true;
    }
    return false;
}

int char_to_int(char input) {
    int char_int = int(input);
    return char_int - 48;
}

char to_lowercase(char input) {
    int char_int = int(input);
    if (is_lowercase(input)) {
        return input;
    }
    return char(char_int + 32);
}

void display_board(std::bitset<64> board) {
    for (int i = 0; i < board.size(); i++) {
        if (i % 8 == 0) {
            std::cout << "\n";
        }
        std::cout << board[i];
    }
}