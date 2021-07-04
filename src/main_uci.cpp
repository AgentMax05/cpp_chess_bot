#include <iostream>
#include "UCI.h"

using namespace std;
int main(int argc, char** argv) {
    UCI uci;
    cout << "Starting UCI" << endl;
    uci.run();
    cout << "Stopped UCI" << endl;
}