# OmNomNum

Implements most of the logic from [numerizer](https://github.com/jduff/numerizer) in c using [re2c](http://re2c.org/) and [lemon](http://www.hwaci.com/sw/lemon/).

## Differences between Numerizer and OmNomNum

- OmNomNum preserves whitespace, Numerizer does not.
- OmNomNum supports an optional `parse_second` parameter, Numerizer does not.
- OmNomNum can support most currency formats, Numerizer does not.
- OmNomNum supports fractions (including mixed fractions, word-based fractions, and numeric fractions like "1 1/2"), with runtime toggle and optional reduction to lowest terms.

## Prereqs

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

To run the benchmarks:

- You need to make and install [google benchmark](https://github.com/google/benchmark).
    - $ `cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true`
    - $ `make`
    - $ `sudo make install`
    - before running, you should set your cpu governor to performance:
        - `sudo cpufreq-set -c \# -g performance`
        - you can switch back to `ondemand`
        - bonus: set up a function to change for all cores, e.g. `cpup performance`

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
make regen
```

#### Installing lemon and re2c from source

If lemon and re2c are not available via your package manager, you can build them from source:

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

Verify installation:
```bash
lemon -? 2>&1 | head -5
re2c --version
```

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
