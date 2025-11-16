# OmNomNum Performance Investigation Notes

This document captures what we measured, how to reproduce it, and a plan to attribute the 2x+ speedup observed in recent commits.

## Summary (what we saw)

- Major speedup occurs between these commits:
  - Before: `0575cc787f85…` (Restore whitespace separator behavior; parser reentrancy + minor perf)
  - After: `455fda3e5973…` (post-quads; parser/scanner changes)
- Benchmarks (mean real_time, nanoseconds unless noted):
  - `0575cc7` → `455fda3` deltas
    - BM_simple: 646 → 176 ns (−72.7%, ~3.7x)
    - BM_long_string: 2667 → 1027 ns (−61.5%)
    - BM_many_numbers: 93.5 µs → 31.8 µs (−66%)
- Scanner‑first fractions (at `c90bb9c…`) are not the root cause of the 2x+ speedup. The big win comes later, from parser/scanner changes.

Collected JSONs (example paths):

- `bench_runs/1-df0cc25084fb…json` (very old commit; built with compatibility patches)
- `worktrees/temp-c90/bench_runs/c90-baseline.json` (scanner‑first fractions)
- `worktrees/temp-c90/bench_runs/c90-direct-append.json` (micro-opt: direct append)
- `bench_runs/0575cc7.json` (pre-big change)
- `test/gb-post-quads.json` (post-change snapshot)
- `bench_runs/HEAD.json` (current)

## Pre‑requisites

- Tools installed and discoverable in PATH or Homebrew:
  - `lemon`, `re2c`
  - `google-benchmark` (Homebrew) and `yaml-cpp` (Homebrew)
- This repo includes convenience scripts:
  - `scripts/benchmark_current.sh` – build+run benchmark for current tree → JSON
  - `scripts/gb_compare_refs.sh` – run benchmarks for two refs using temporary worktrees
  - `scripts/gb_sweep_range.sh` – iterate a range of commits, build+run, save JSONs (added here)
  - `scripts/gb_extract_means.sh` – extract mean real_time from JSONs (added here)

Compatibility notes for very old commits (e.g., `df0cc25…`):

- Older Makefiles hard-code paths for `lemon`/`makeheaders` and lack modern include/lib flags.
- We patched the old worktree Makefile to:
  - Use `LEMON?=$(command -v lemon)` and ignore non-zero exit from lemon conflict warnings.
  - Replace the `makeheaders` step with a generated `parser.h` (mirrors current approach).
  - Add `BENCH_PREFIX` and `YAML_PREFIX` Homebrew prefixes for includes/libs.
  - Include `parser_compat.c` to provide the `ParseReset` wrapper lemon didn’t emit back then.

These patches live only in the temporary worktrees to make those refs build; the main history is unchanged.

## Reproduce: sweep and compare

Run a sweep from an old ref to HEAD and save all JSONs under `bench_runs/`:

```
bash scripts/gb_sweep_range.sh df0cc25084fb HEAD bench_runs
```

Extract a CSV of means from the JSONs:

```
bash scripts/gb_extract_means.sh bench_runs/*.json > bench_runs/means.csv
```

The CSV format is:

```
file,BM_simple_ns,BM_long_string_ns,BM_many_numbers_ns
```

## What led to the speedup

- Micro-optimizations tested on `c90bb9c…` (scanner‑first fractions):
  - Pre‑reserve number list capacity and result buffer: negligible effect.
  - Render numbers directly into the final buffer: small win (~5% BM_simple, ~2% long string).
- The 2x+ improvement appears only after the parser/scanner changes present by `455fda3…`.

## Next: Attribute the win precisely

Approach: start from a baseline (e.g., `0575cc7…` or `c90bb9c…`) and incrementally apply changes from `455fda3…` to find which chunk(s) move the needle. Focus files: `parser.yy` and `scanner.re`.

Suggested process:

1) Create a scratch branch/worktree at the baseline commit.
   - `git worktree add -b perf-chop worktrees/perf-chop 0575cc7`

2) Build baseline benchmark JSON.
   - `bash worktrees/perf-chop/scripts/benchmark_current.sh bench_runs/baseline.json`

3) Cherry-pick logical chunks from `455fda3…` onto the baseline.
   - Use `git checkout 455fda3 -- parser.yy scanner.re` then stage hunks with `git add -p`.
   - Or split the diff into topical patches (e.g., token fast paths, grammar reduction paths, reset semantics).

4) After each minimal change, rebuild and run the benchmark.
   - `bash worktrees/perf-chop/scripts/benchmark_current.sh bench_runs/chunk-XX.json`
   - Append results into a running CSV using `scripts/gb_extract_means.sh`.

5) If a change is behind a feature toggle, wire a guard macro to flip it on/off in the same ref:
   - Add `#ifndef PERF_TOGGLE_X … #endif` around the new code path.
   - Rebuild benchmarks with `CFLAGS+=-DPERF_TOGGLE_X=1` to measure impact without changing history.

6) Once a change shows a large delta, drill in further within that hunk to identify the minimal driver.

Tips:
- Keep lemon/re2c outputs stable between runs: only touch `.re` and `.yy` when measuring scanner/parser changes.
- For noisy results, increase `--benchmark_min_time` to 5s and average over more repetitions.

## Known good commit set

These commit IDs produced JSONs in this investigation and are good anchors:

- `df0cc25084fb…` (old) → requires worktree Makefile compatibility patches to build.
- `c90bb9cce940…` (scanner-first fractions).
- `0575cc787f85…` (pre-change baseline) → builds with current Makefile template.
- `455fda3e5973…` (post-change; big win).
- `574d8e30bdc6…` (HEAD at the time of writing).

## Files added by this investigation

- `scripts/gb_sweep_range.sh` – commit range runner (worktrees + JSON).
- `scripts/gb_extract_means.sh` – JSON → CSV for three benchmarks.

These are non-invasive helpers to repeat the study.

