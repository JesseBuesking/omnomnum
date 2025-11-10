# OmNomNum

Implements most of the logic from [numerizer](https://github.com/jduff/numerizer) in c using [re2c](http://re2c.org/) and [lemon](http://www.hwaci.com/sw/lemon/).

## Differences between Numerizer and OmNomNum

- OmNomNum preserves whitespace, Numerizer does not.
- OmNomNum supports an optional `parse_second` parameter, Numerizer does not.
- OmNomNum can support most currency formats, Numerizer does not.
- OmNomNum supports fractions (including mixed fractions, word-based fractions, and numeric fractions like "1 1/2"), with runtime toggle and optional reduction to lowest terms.

## Prereqs

### Automated Setup (Recommended)

For **lemon**, **re2c**, and **Google Benchmark**, use the automated setup script:

```bash
# Option 1: Install and add to current shell session
source scripts/setup_environment.sh

# Option 2: Install and optionally add to ~/.bashrc or ~/.zshrc
bash scripts/setup_environment.sh
```

The script will:
- Check for and install missing tools (lemon, re2c, Google Benchmark)
- Install to local `tools/` directory (no sudo required)
- Verify installations
- Optionally configure your shell for permanent access

**Manual setup alternative:** See "Installing lemon and re2c from source" section below for manual installation steps.

### Claude Code Users - Important!

**TL;DR:** Always prefix build commands with `source tools/env.sh &&`

Claude Code runs each Bash command in a **new shell session**. This means environment variables (like PATH) don't persist between commands.

**First time setup:**
```bash
source scripts/setup_environment.sh
```

**Every build command:**
```bash
# ❌ WRONG - tools won't be found
source tools/env.sh
make

# ✅ CORRECT - same shell session
source tools/env.sh && make
```

**Common commands:**
```bash
# Build
source tools/env.sh && make -j4

# Rebuild from scratch
source tools/env.sh && make clean && make regen && make -j4

# Run tests
source tools/env.sh && make test

# Run benchmarks
source tools/env.sh && make benchmark
```

**Why?** Each `Bash` tool invocation creates a fresh shell. The `&&` operator chains commands in the same shell, preserving the environment.

See [docs/CLAUDE_CODE.md](docs/CLAUDE_CODE.md) for detailed workflow and session start instructions.

### Test Dependencies (Manual Installation Required)

To run the tests:

- You need to make and install [yaml-cpp](https://github.com/jbeder/yaml-cpp):
    ```bash
    cd /tmp
    git clone --depth 1 https://github.com/jbeder/yaml-cpp.git
    cd yaml-cpp
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DYAML_BUILD_SHARED_LIBS=OFF
    make -j4
    sudo make install
    ```

- You need to make and install [googletest](https://github.com/google/googletest):
    ```bash
    cd /tmp
    git clone --depth 1 https://github.com/google/googletest.git
    cd googletest
    mkdir build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j4
    sudo make install
    ```

### Performance Optimization (Optional)

For best benchmark results, set your CPU governor to performance mode:
```bash
sudo cpufreq-set -c # -g performance  # Replace # with core number
```

Switch back to default:
```bash
sudo cpufreq-set -c # -g ondemand
```

Pro tip: Create a function to change all cores at once (add to `~/.bashrc`):
```bash
cpup() { for i in {0..$(nproc --ignore=1)}; do sudo cpufreq-set -c $i -g $1; done; }
# Usage: cpup performance
```

## Building

### Quick Build (no lemon/re2c required)
The generated parser and scanner files are included in the repository for easy building:
```bash
make
```

### Building with CMake (Recommended for cross-platform)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

See [CMAKE.md](CMAKE.md) for detailed build instructions and options.

### Regenerating Parser/Scanner (requires lemon and re2c)

If you modify `parser.yy` or `scanner.re`:
```bash
# Ensure tools are available (if not using automated setup)
source tools/env.sh  # or: source scripts/setup_environment.sh

# Regenerate parser and scanner
make regen
```

#### Manual Installation of lemon and re2c (Advanced)

**Note:** The recommended way is to use `scripts/setup_environment.sh` (see Prereqs section above).

If you prefer manual installation or need to install system-wide, you can build from source:

**lemon** (from SQLite):
```bash
cd /tmp
git clone --depth 1 https://github.com/mackyle/sqlite.git sqlite-mirror
cd sqlite-mirror/tool
cc -o lemon lemon.c
sudo cp lemon /usr/local/bin/
sudo cp lempar.c /usr/local/bin/
```

**re2c** (lexer generator):
```bash
cd /tmp
git clone --depth 1 https://github.com/skvadrik/re2c.git
cd re2c
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
sudo cp re2c /usr/local/bin/
```

**Google Benchmark** (for benchmarking):
```bash
cd /tmp
git clone --depth 1 https://github.com/google/benchmark.git
cd benchmark
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true -DBENCHMARK_ENABLE_TESTING=OFF
make -j4
sudo make install
```

Verify installation:
```bash
lemon -x  # Should print version
re2c --version
pkg-config --modversion benchmark
```

## Benchmarking

OmNomNum includes comprehensive benchmarking support using Google Benchmark.

**Prerequisites:** Google Benchmark must be installed. Use `source scripts/setup_environment.sh` (recommended) or see manual installation above.

You can run benchmarks using three optimized configurations:

### Quick Start

```bash
# Fast mode: Quick iteration during development (~9s)
make benchmark-fast

# Default mode: Recommended for CI/CD (~32s)
make benchmark

# Accurate mode: For releases and validation (~64s)
make benchmark-accurate
```

### Benchmark Modes

Three configurations have been thoroughly tested and validated for reliability:

| Mode | min_time | reps | Runtime | CV of Means | Use Case |
|------|----------|------|---------|-------------|----------|
| **Fast** ⚡ | 0.05s | 5 | ~9s | 2.11% | Quick development iteration |
| **Default** ⭐ | 0.1s | 10 | ~32s | 2.02% | CI/CD, pre-merge testing (recommended) |
| **Accurate** 🏆 | 0.2s | 10 | ~64s | 1.40% | Official releases, performance validation |

**CV of Means** measures reproducibility - how consistent results are across runs. All modes achieve <3% CV, ensuring reliable regression detection.

### Advanced Usage

**Direct script usage:**
```bash
# Fast mode
BENCH_MODE=fast bash scripts/benchmark_current.sh output.json

# Default mode (or no flag)
bash scripts/benchmark_current.sh output.json

# Accurate mode
BENCH_MODE=accurate bash scripts/benchmark_current.sh output.json
```

**Mean comparison mode** - Run benchmarks multiple times to measure reproducibility:
```bash
# Run 5 independent benchmark runs and compare means
make benchmark-compare COMPARE_RUNS=5

# Or via script directly
COMPARE_MODE=1 COMPARE_RUNS=5 bash scripts/benchmark_current.sh output.json
```

This will analyze the coefficient of variation (CV) of means across runs and report:
- Average CV of means (target: <3%)
- Maximum pairwise differences
- Pass/fail based on reliability thresholds

**Custom configurations:**
```bash
# Override min_time and reps
BENCH_MIN_TIME=0.3s BENCH_REPS=15 bash scripts/benchmark_current.sh output.json
```

**Legacy support:**
```bash
# QUICK_BENCH=1 maps to fast mode for backward compatibility
QUICK_BENCH=1 bash scripts/benchmark_current.sh output.json
```

### Performance Tips

For most reliable results:
1. Set CPU governor to performance mode (see installation section above)
2. Close unnecessary background applications
3. Run on a quiet system (minimal other processes)
4. For release validation, use `make benchmark-accurate`

See [BENCHMARK_RECOMMENDATIONS.md](BENCHMARK_RECOMMENDATIONS.md) for detailed analysis of configuration testing and reliability metrics.

## Development Scripts

OmNomNum includes several helper scripts in the `scripts/` directory to streamline development:

### Environment Setup
- **`scripts/setup_environment.sh`** - Automated installation of development tools
  - Installs lemon, re2c, and Google Benchmark to local `tools/` directory
  - Can be sourced (`source scripts/setup_environment.sh`) to add tools to PATH immediately
  - Can be executed (`bash scripts/setup_environment.sh`) with optional shell config integration
  - No sudo required - installs locally
  - See [Prereqs](#prereqs) section for usage details

### Claude Code Integration
- **`.claude/hooks/session-start.sh`** - Automatic tool setup for Claude Code sessions
  - Runs automatically when opening the project in Claude Code
  - Ensures development tools are always available without manual intervention
  - Sources the environment setup script to configure PATH

### Benchmarking Scripts
- **`scripts/benchmark_current.sh`** - Run benchmarks on current codebase
- **`scripts/benchmark_all_commits.sh`** - Benchmark across multiple commits
- **`scripts/benchmark_runner.sh`** - Core benchmark execution logic
- **`scripts/gb_compare.sh`** - Compare Google Benchmark results
- See [scripts/README.md](scripts/README.md) for detailed script documentation

### Code Generation
- **`scripts/regenerate_gperf.sh`** - Regenerate perfect hash for denominator word lookup
  - Automatically updates `scanner.re` with gperf-generated code
  - Rebuilds scanner and verifies tests pass
  - See [scripts/README.md](scripts/README.md) for usage details

## Features

- **Thread-safe**: Parser re-entrancy with no global state
- **Fraction support**: Parse word-based ("one half"), mixed ("1 1/2"), and numeric fractions with optional reduction to lowest terms
- **Runtime toggles**: Control fraction parsing, ordinal parsing, and percent normalization at runtime
- **Percent handling**: Normalize "percent" to "%" or convert to decimal (e.g., "50%" → "0.5")
- **Performance optimized**: Fast numeric parsing, minimal allocations
- **Cross-platform**: CMake support for Linux, macOS, and Windows/MSYS2

## Completed Enhancements

- [x] Thread safety and parser re-entrancy
- [x] Runtime toggles for fraction and ordinal parsing
- [x] Comprehensive fraction support (word-based, mixed numeric, expanded denominators)
- [x] Fast numeric parsing (replaced sscanf with strtod)
- [x] Architecture-specific optimizations (SSE4.2 gating)
- [x] Stable parser.h generation (checked-in generated files)
- [x] CLI utility with flags for precision, parsing options
- [x] Extensive test coverage (263+ test cases)
- [x] Optional fraction reduction via GCD
- [x] CMake build system with cross-platform support
- [x] Percent unit normalization (symbol and decimal conversion)

## Recent Additions (Latest Session)

- [x] **"minus" word sign support**: "minus five" → "-5" (Task 11)
- [x] **Fixed parse_fractions runtime toggle**: Fallback path now respects --no-parse-fractions flag
- [x] **Robust word-to-number mapping**: "5 thousand and three" → "5003" (Task 13)

## Future Enhancements

See [TASKS.md](TASKS.md) for detailed task tracking and implementation notes.

Potential future work:
- [ ] Scientific notation support (e.g., "1.5e10")
- [ ] Extended number word mappings for larger ranges

## Notes

### Multiple numbers

Multiple numbers back-to-back causes this grammar to be ambiguous (see the rule
`numbers ::= numbers number.`). I believe it's not possible to resolve this. For
example, take "one two" and "one thousand one hundred". Let's assume that you're
on "two" in the first example, and the second "one" in the second example. It's
not possible for the  parser to use any reduce rules in the second example
because it doesn't yet know if there's more data available to match another
rule -- the "hundred" -- or if it's safe to reduce like in the first example. I
may be wrong, but this is my current understanding of why it's ambiguous.
