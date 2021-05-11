<h1 align=center>C++ Chess Bot</h1>

<h3 align=center>A simple chess engine.</h3>

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

# Continuous Integration (CI)

There is a [GitHub Actions](https://github.com/features/actions) workflow
defined in `.github/workflows` that will run the tests on Ubuntu, Windows,
and macOS.
