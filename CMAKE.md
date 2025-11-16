# CMake Build System for OmNomNum

This document describes how to build OmNomNum using CMake for cross-platform builds.

## Quick Start

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests (if available)
ctest

# Install (optional)
sudo cmake --install .
```

## Requirements

### Required
- CMake 3.12 or higher
- C99-compatible C compiler
- C++17-compatible C++ compiler

### Optional (for tests)
- Google Test (`libgtest-dev`)
- yaml-cpp (`libyaml-cpp-dev`)

### Optional (for benchmarks)
- Google Benchmark (`libbenchmark-dev`)

### Optional (for regenerating parser/scanner)
- lemon (parser generator)
- re2c (scanner generator)

## Installing Dependencies

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake build-essential
sudo apt-get install libgtest-dev libyaml-cpp-dev  # for tests
sudo apt-get install libbenchmark-dev              # for benchmarks
sudo apt-get install lemon re2c                     # for regeneration
```

### macOS (Homebrew)
```bash
brew install cmake
brew install googletest yaml-cpp google-benchmark
brew install lemon re2c
```

### Windows (MSYS2)
```bash
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtest
pacman -S mingw-w64-x86_64-yaml-cpp
```

## Build Options

CMake options can be set via `-D` flags:

```bash
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release ..
```

### Available Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | `Release` | Build type: `Release`, `Debug`, `RelWithDebInfo`, `MinSizeRel` |
| `SCANNER_FRACTIONS` | `ON` | Enable scanner-side fraction recognition |
| `BUILD_TESTS` | `ON` | Build test suite (requires GTest and yaml-cpp) |
| `BUILD_BENCHMARKS` | `OFF` | Build benchmarks (requires Google Benchmark) |
| `USE_LTO` | `OFF` | Enable Link Time Optimization |

## Build Types

### Release Build (default)
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
- Optimized for performance (`-O3`)
- No debug symbols
- Defines `NDEBUG`

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```
- Debug symbols (`-g -ggdb`)
- Lower optimization (`-O1`)
- Defines `DEBUG` and `print_errors`

### With LTO
```bash
cmake -DUSE_LTO=ON ..
cmake --build .
```
- Enables Link Time Optimization for better performance
- May increase build time

## Building Tests

Tests are built by default if GTest and yaml-cpp are found:

```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
cmake --build .
ctest                  # Run all tests
ctest --verbose        # Run with verbose output
ctest --output-on-failure  # Show output only for failures
```

To run individual tests:
```bash
./test_omnomnum
./test_util
```

## Building Benchmarks

```bash
mkdir build && cd build
cmake -DBUILD_BENCHMARKS=ON ..
cmake --build .
./test_benchmark
```

## Regenerating Parser and Scanner

If you have `lemon` and `re2c` installed and want to regenerate the parser/scanner from source:

```bash
mkdir build && cd build
cmake ..
cmake --build . --target regen
```

This will regenerate:
- `parser.c` and `parser.h` from `parser.yy`
- `scanner.c` from `scanner.re`

**Note:** Generated files are checked into git, so regeneration is optional and only needed when modifying grammar files.

## Cross-Platform Notes

### Linux
- Standard build works out of the box
- SSE4.2 optimizations enabled on x86/x64 architectures

### macOS
- ARM64 (Apple Silicon) and x86_64 supported
- SSE4.2 only enabled on Intel Macs
- Use Homebrew for dependencies

### Windows (MSYS2/MinGW)
- Build in MSYS2 MinGW64 shell
- All features supported
- Use MSYS2 package manager for dependencies

## Installation

To install system-wide:

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
cmake --build .
sudo cmake --install .
```

This installs:
- Binary: `$PREFIX/bin/omnomnum`
- Headers: `$PREFIX/include/omnomnum/*.h`

## Troubleshooting

### Tests not building
- Ensure GTest and yaml-cpp are installed
- Check CMake output for "GTest not found" or "yaml-cpp not found"
- On Ubuntu, you may need to build GTest from source in `/usr/src/gtest`

### LTO errors
- Disable LTO with `-DUSE_LTO=OFF`
- Some compilers/linkers don't support LTO

### Parser/Scanner regeneration fails
- Ensure `lemon` and `re2c` are in PATH
- Generated files are checked in, so regeneration is optional
- Use existing generated files if tools unavailable

## Examples

### Minimal build (no tests, no benchmarks)
```bash
mkdir build && cd build
cmake -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=OFF ..
cmake --build .
./omnomnum --help
```

### Full development build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_BENCHMARKS=ON ..
cmake --build .
ctest
```

### Optimized release with LTO
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_LTO=ON ..
cmake --build .
sudo cmake --install .
```
