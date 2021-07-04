#include <gtest/gtest.h>
#include <iostream>
#include "UCI.h"
#include <vector>
using namespace std;

TEST(UCITest, ReturnsUCIOk) {
    UCI uci;
    vector<string> output = uci.run_command("uci");
    string last_line = output[output.size() - 1];
    EXPECT_EQ("uciok", last_line);
}
