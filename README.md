<h1 align=center>C++ Chess Bot</h1>

<h3 align=center>A simple chess engine.</h3>

# Usage

`chess.exe [options]`

- `-h` Shows the available options
- `-f <FEN|default>` Allows non-interactive FEN entry
- `-b <unicode|ascii>` Allows forcing the board display mode. Defaults to Ascii on Windows and Unicode elsewhere.
- `-c <white|black>` Allows setting the color used. Defaults to white.
- `-d <depth>` Allows setting the move search depth. Defaults to 8.

# Testing

## Setup CMake

macOS:

```
brew install cmake
```

## Running

Run `cmake` with the source directory `.` and the build
directory `build`. Then, "build a CMake-generated project binary tree".
Finally, use `build` as the working directory and run `ctest`.

```
cmake -S . -B build
cmake --build build
cd build && ctest
```

Building the tests requires C++17 for use of the `filesystem` built-in
namespace to perform path building.

# Continuous Integration (CI)

There is a [GitHub Actions](https://github.com/features/actions) workflow
defined in `.github/workflows` that will run the tests on Ubuntu, Windows,
and macOS.
