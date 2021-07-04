cmake_minimum_required(VERSION 3.14)
project(chess_bot)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(src)

file(GLOB SRC_FILES src/*.cpp)

add_executable(
  chess
  src/board.cpp
  src/eval.cpp
  src/misc_funcs.cpp
  src/move_generation.cpp
  src/minimax.cpp
  src/check_legal.cpp
  src/set_attacking.cpp
  src/BoardRenderer.cpp
  src/main.cpp
)

add_executable(
  chess_uci
  src/board.cpp
  src/eval.cpp
  src/misc_funcs.cpp
  src/move_generation.cpp
  src/minimax.cpp
  src/check_legal.cpp
  src/set_attacking.cpp
  src/BoardRenderer.cpp
  src/UCI.cpp
  src/main_uci.cpp
)

add_executable(
  all_tests
  tests/eval_test.cpp
  tests/move_generation_test.cpp
  tests/minimax_test.cpp
  tests/board_test.cpp
  tests/uci_test.cpp
  src/board.cpp
  src/eval.cpp
  src/misc_funcs.cpp
  src/move_generation.cpp
  src/minimax.cpp
  src/check_legal.cpp
  src/set_attacking.cpp
  src/BoardRenderer.cpp
  src/UCI.cpp
)
target_link_libraries(
  all_tests
  gtest_main
)
include(GoogleTest)
gtest_discover_tests(all_tests)
