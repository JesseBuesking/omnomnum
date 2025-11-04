# OmNomNum – Workable Task List

This is a concrete, prioritized task list to continue improving correctness, performance, and DX. Each item includes scope and acceptance criteria to keep work tight and verifiable.

## 1) Parser Re-entrancy and Thread Safety
- Replace global `pParser` and `numberHolder` with fields owned by a request/context (e.g., in `ParserState` or a new `OmNomCtx`).
- Allocate parser with `ParseAlloc` per call; free with `ParseFree` after use.
- Acceptance: parallel calls to `normalize()` on different threads produce correct results and do not race or corrupt output.

## 2) Runtime Toggle: Fraction Parsing
- Add `parse_fractions` to `ParserState` and honor it in the scanner (guard fraction token emission at runtime in addition to `SCANNER_FRACTIONS`).
- Expose in CLI/main and tests.
- Acceptance: With `parse_fractions=false`, inputs like `"one eighth"` and `"1 1/2"` remain unchanged; with true, they normalize as fractions.

## 3) Harden Fraction Word Matching
- Enforce word boundaries for mixed-word fractions (ensure `and` is a standalone word).
- Expand denominator vocabulary (tenth(s), eleventh(s), twelfth(s), …) and add tests.
- Acceptance: New test cases for expanded denominators pass; no false positives inside larger words.

## 4) Numeric Parsing Fast Path
- Replace `sscanf`/temporary `sds` conversions in scanner numeric rules with `strtod` or a fast, bounded parser to avoid allocations.
- Acceptance: Benchmarks show equal or improved timings for DECIMAL_* cases; no change in correctness.

## 5) Remove/Condition `-msse4.2`
- Drop or gate `-msse4.2` by architecture; eliminate warnings.
- Acceptance: No compiler warnings about unused flags; performance unchanged.

## 6) Stabilize `parser.h` Generation
- Either: keep a known-good `parser.h` under version control and remove in-Makefile generation; or enhance generator to mirror Lemon’s template robustly.
- Acceptance: `make clean && make test` works without depending on host Lemon quirks; CI proves repeatable builds.

## 7) CLI Utility
- Add a tiny CLI (e.g., `omn`) with: `--precision`, `--parse-second`, `--parse-fractions`, reading stdin/files.
- Acceptance: Running `omn <file>` prints normalized text; help and examples included.

## 8) Tests: Add Coverage for New Fractions
- Add cases for negative fractions, mixed numeric with trailing text, expanded denominators, and runtime toggle behavior.
- Acceptance: `make test` passes with added cases; coverage of fraction paths increases.

## 9) Optional: Fraction Reduction (Opt-in)
- Provide a flag to reduce fractions (gcd) while keeping current behavior default (non-reduced).
- Acceptance: With reduction enabled, `"two fourths"` → `"1/2"`; disabled remains `"2/4"`.

## 10) Optional: CMake Build
- Introduce CMake for cross-platform builds (Linux CI, Windows/MSYS2); find re2c, lemon, gtest, yaml-cpp, google-benchmark via `find_package` or variable hints.
- Acceptance: `cmake .. && cmake --build . && ctest` succeeds; documentation explains options.

---

Notes
- Current state: scanner-driven fractions implemented; post-pass removed; tests pass (114/114). Google Benchmark targets produce JSON (before/after), with `SCANNER_FRACTIONS` providing apples-to-apples comparisons.
- Environment caveat: On some macOS sandboxes, the Google Benchmark binary may require running outside the sandbox; the `bench_local` fallback remains available.

