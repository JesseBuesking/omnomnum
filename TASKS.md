# OmNomNum – Workable Task List

This is a concrete, prioritized task list to continue improving correctness, performance, and DX. Each item includes scope and acceptance criteria to keep work tight and verifiable.

## 1) Parser Re-entrancy and Thread Safety
- Status: Completed
- Replace global `pParser` and `numberHolder` with fields owned by a request/context (e.g., in `ParserState` or a new `OmNomCtx`).
- Allocate parser with `ParseAlloc` per call; free with `ParseFree` after use.
- Acceptance: parallel calls to `normalize()` on different threads produce correct results and do not race or corrupt output.
Implementation Notes: `ParserState` now owns `pParser` and a scratch `numberHolder`; parser is allocated on first use and reset per run (see `scanner.def.h`, `scanner.def.c`, and `omnomnum.c`).

## 2) Runtime Toggle: Fraction Parsing
- Status: Completed
- Add `parse_fractions` to `ParserState` and honor it in the scanner (guard fraction token emission at runtime in addition to `SCANNER_FRACTIONS`).
- Expose in CLI/main and tests.
- Acceptance: With `parse_fractions=false`, inputs like `"one eighth"` and `"1 1/2"` remain unchanged; with true, they normalize as fractions.
Implementation Notes: Added `ParserState.parse_fractions` (default true). All fraction-emitting scanner rules are gated by this flag when `SCANNER_FRACTIONS` is enabled. CLI now exposes `--no-parse-fractions` flag, and dedicated tests have been added (test/test_omnomnum.c:129-199) for both enabled and disabled modes.

## 3) Harden Fraction Word Matching
- Status: Completed
- Enforce word boundaries for mixed-word fractions (ensure `and` is a standalone word).
- Expand denominator vocabulary (tenth(s), eleventh(s), twelfth(s), …) and add tests.
- Acceptance: New test cases for expanded denominators pass; no false positives inside larger words.
Implementation Notes: Expanded `map_denom_word` to include tenth through nineteenth (10-19), -ty forms (twentieth through ninetieth: 20, 30, 40, 50, 60, 70, 80, 90), and large denominators (thousandth, millionth, billionth, trillionth). Updated scanner.re fraction rules to recognize all new denominators. Word boundaries are enforced by re2c patterns using `WS+` around 'and', preventing matches inside larger words. Added 44 new test cases in cases.yml covering simple fractions, mixed fractions, and word boundary verification. All 203 tests pass.

## 4) Numeric Parsing Fast Path
- Replace `sscanf`/temporary `sds` conversions in scanner numeric rules with `strtod` or a fast, bounded parser to avoid allocations.
- Acceptance: Benchmarks show equal or improved timings for DECIMAL_* cases; no change in correctness.

## 5) Remove/Condition `-msse4.2`
- Status: Completed
- Drop or gate `-msse4.2` by architecture; eliminate warnings.
- Acceptance: No compiler warnings about unused flags; performance unchanged.
Implementation Notes: Makefile gates `-msse4.2` behind x86 architectures only.

## 6) Stabilize `parser.h` Generation
- Status: Completed
- Either: keep a known-good `parser.h` under version control and remove in-Makefile generation; or enhance generator to mirror Lemon's template robustly.
- Acceptance: `make clean && make test` works without depending on host Lemon quirks; CI proves repeatable builds.
Implementation Notes: Generated files (parser.c, parser.h, scanner.c) are now checked into git for stable builds. Makefile updated with graceful fallback when lemon/re2c are unavailable - it uses the checked-in versions. Added `make regen` target for developers who need to regenerate from parser.yy/scanner.re. Added `make distclean` for deep cleaning. Standard `make clean` now preserves generated files, preventing accidental deletion of files needed for builds without lemon/re2c installed.

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


## 11) Minus (Word) Sign Support
- Problem: The word "minus" is currently treated as plain text; only the word "negative" or a leading '-' acts as a sign. This creates inconsistent behavior across inputs like "minus five" (unchanged) vs "negative five" (→ -5).
- Proposal: Treat the word "minus" as a sign (same as `NEGATIVE`) when it precedes a `final_number` without intervening non-separator characters.
- Scope:
  - Scanner: add a `TOKEN_MINUS` for the word "minus".
  - Parser: add rule `number ::= MINUS final_number` mirroring `NEGATIVE` logic (propagate negativity to values and fractions).
  - Edge handling: ensure we don't capture subtraction semantics (we don't parse arithmetic), and preserve "minus" when not followed by a number (e.g., named phrases or hyphenated words).
- Acceptance:
  - "minus five" → "-5"; "minus 1 1/2" → "-3/2"; "minus one point five" → "-1.5".
  - "minus sign" or "minus-two" inside words remains unchanged.

## 12) Percent Unit Semantics
- Problem: "percent" and "%" are currently preserved as-is, even when numbers are normalized (e.g., "two and a half percent" → "5/2 percent"). Desired behavior may vary: keep as a unit, convert to symbol, or normalize as a decimal [0,1].
- Options:
  1) Unit-preserving (status quo): continue leaving "percent"/"%" unchanged after number normalization.
  2) Symbol canonicalization: normalize the word form to the symbol (e.g., "percent" → "%"), preserving spacing rules.
  3) Decimal-of-one conversion (opt-in): convert n percent → n/100 (e.g., "50 percent" → "0.5"). Combine with fraction support (e.g., "1/2 percent" → "0.005").
- Scope:
  - Introduce `ParserState` flags: `normalize_percent_symbol`, `percent_as_decimal`.
  - Add a light post-pass when a number is immediately followed by "percent" or "%".
  - Locale spacing: prefer no space before "%" (e.g., "50%").
- Acceptance:
  - With defaults: behavior matches current outputs.
  - With `normalize_percent_symbol=true`: "50 percent" → "50%"; "one point five percent" → "1.5%".
  - With `percent_as_decimal=true`: "50%" → "0.5"; "two and a half percent" → "0.025" (or as fraction if configured).
