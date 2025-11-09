#!/usr/bin/env bash
set -euo pipefail

# Build and run Google Benchmark for the current working tree,
# writing results to the provided JSON path.
#
# Usage:
#   bash scripts/benchmark_current.sh [OUTPUT_JSON]
#   COMPARE_MODE=1 bash scripts/benchmark_current.sh [OUTPUT_JSON]  # Run multiple times and compare means
#   COMPARE_RUNS=N bash scripts/benchmark_current.sh [OUTPUT_JSON]  # Number of runs for comparison (default: 2)
#
# Defaults to test/gb-<timestamp>.json when OUTPUT_JSON is omitted.

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

timestamp="$(date +%Y%m%d-%H%M%S)"
OUT_JSON="${1:-test/gb-${timestamp}.json}"

# Parse comparison mode flags
COMPARE_MODE="${COMPARE_MODE:-0}"
COMPARE_RUNS="${COMPARE_RUNS:-2}"

echo "[bench_current] Repo:  $ROOT_DIR"
echo "[bench_current] JSON:  $OUT_JSON"
if [[ "$COMPARE_MODE" == "1" ]]; then
  echo "[bench_current] Mode:  COMPARE ($COMPARE_RUNS runs)"
fi

# Compute absolute output path
to_abs_path() {
  case "$1" in
    /*) printf "%s" "$1" ;;
     *) printf "%s/%s" "$ROOT_DIR" "$1" ;;
  esac
}
OUT_JSON_ABS="$(to_abs_path "$OUT_JSON")"

mkdir -p "$(dirname "$OUT_JSON_ABS")"

# Locate google-benchmark prefix if not provided
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  if command -v brew >/dev/null 2>&1; then
    BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
  fi
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[bench_current] Error: google-benchmark not found. Install via Homebrew or set BENCH_PREFIX."
  echo "                e.g., BENCH_PREFIX=\"$(brew --prefix google-benchmark 2>/dev/null || echo /usr/local/opt/google-benchmark)\" bash $0 <OUTPUT_JSON>"
  exit 1
fi
echo "[bench_current] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"

FREEZE=${FREEZE_CODEGEN:-0}
if [[ "$FREEZE" == "1" ]]; then
  echo "[bench_current] FREEZE_CODEGEN=1: restoring generated files and touching"
  if git ls-files --error-unmatch parser.c >/dev/null 2>&1; then git checkout -- parser.c || true; fi
  if git ls-files --error-unmatch parser.h >/dev/null 2>&1; then git checkout -- parser.h || true; fi
  if git ls-files --error-unmatch scanner.c >/dev/null 2>&1; then git checkout -- scanner.c || true; fi
  touch -c parser.c scanner.c parser.h 2>/dev/null || true
  export LEMON=/usr/bin/false
  export RE2C=/usr/bin/false
fi

ensure_objs() {
  mkdir -p grisu2 branchlut || true
  [[ -f grisu2.o && ! -f grisu2/grisu2.o ]] && mv -f grisu2.o grisu2/grisu2.o || true
  [[ -f branchlut.o && ! -f branchlut/branchlut.o ]] && mv -f branchlut.o branchlut/branchlut.o || true
  if [[ ! -f grisu2/grisu2.o && -f grisu2/grisu2.c ]]; then
    cc -O3 -std=c99 -Wall -W -Wno-missing-field-initializers -I. -c grisu2/grisu2.c -o grisu2/grisu2.o || true
  fi
  if [[ ! -f branchlut/branchlut.o && -f branchlut/branchlut.c ]]; then
    cc -O3 -std=c99 -Wall -W -Wno-missing-field-initializers -I. -c branchlut/branchlut.c -o branchlut/branchlut.o || true
  fi
  # If top-level objects exist (produced by older rules), create symlinks expected by linker
  if [[ -f grisu2.o && ! -f grisu2/grisu2.o ]]; then ln -s ../grisu2.o grisu2/grisu2.o 2>/dev/null || true; fi
  if [[ -f branchlut.o && ! -f branchlut/branchlut.o ]]; then ln -s ../branchlut.o branchlut/branchlut.o 2>/dev/null || true; fi
}

echo "[bench_current] Building benchmark binary ..."

# Set compile flags for quick mode
EXTRA_CXXFLAGS=""
if [[ "${QUICK_BENCH:-0}" == "1" ]]; then
  EXTRA_CXXFLAGS="-DQUICK_BENCH"
fi

ensure_objs
if ! make test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" CXX17FLAGS="-std=c++17 $EXTRA_CXXFLAGS"; then
  echo "[bench_current] Make link failed; ensuring objects and retrying"
  ensure_objs
  if ! make -B test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" CXX17FLAGS="-std=c++17 $EXTRA_CXXFLAGS"; then
    echo "[bench_current] Manual compile+link fallback"
    # Build core objects explicitly (ignore failures; gather what exists)
    make -k parser.o parser_compat.o omnomnum.o scanner.o scan.o sds.o itoa.o dtoa.o scanner.def.o util.o BENCH_PREFIX="$BENCH_PREFIX_ENV" || true
    ensure_objs
    # Compile benchmark object if missing
    if [[ ! -f test/test_benchmark.o && -f test/test_benchmark.c ]]; then
      c++ -std=c++17 -I"$BENCH_PREFIX_ENV"/include -I. \
        -DGIT_SHA=\"$(git rev-parse --short=12 HEAD 2>/dev/null || echo unknown)\" \
        -DGIT_DESC=\"$(git describe --always --dirty --tags 2>/dev/null || git rev-parse --short=12 HEAD 2>/dev/null || echo unknown)\" \
        -DBUILD_TIME=\"$(date -u +%FT%TZ)\" \
        -DTREE_DIRTY_STR=\"$(test -n "$(git status --porcelain 2>/dev/null)" && echo 1 || echo 0)\" \
        $EXTRA_CXXFLAGS \
        -c test/test_benchmark.c -o test/test_benchmark.o
    fi
    # Link with whatever objects we have; include top-level and subdir objects
    objs=$(ls -1 *.o grisu2/*.o branchlut/*.o 2>/dev/null | tr '\n' ' ')
    echo "[bench_current] Linking with objs: $objs"
    c++ -std=c++17 -o test/test_benchmark -I. $objs -pthread -L"$BENCH_PREFIX_ENV"/lib -lbenchmark
  fi
fi

echo "[bench_current] Running benchmark -> $OUT_JSON_ABS"

# Quick mode for development (QUICK_BENCH=1): faster iterations with 0.5s min time, 3 reps
# Thorough mode (default): production quality with 2s min time, 10 reps
if [[ "${QUICK_BENCH:-0}" == "1" ]]; then
  echo "[bench_current] Using QUICK mode (0.5s min_time, 3 reps)"
  MIN_TIME="0.5s"
  REPS=3
else
  echo "[bench_current] Using THOROUGH mode (2s min_time, 10 reps)"
  MIN_TIME="2s"
  REPS=10
fi

# Allow override of min_time and reps via environment variables
MIN_TIME="${BENCH_MIN_TIME:-$MIN_TIME}"
REPS="${BENCH_REPS:-$REPS}"

if [[ "$COMPARE_MODE" == "1" ]]; then
  # Comparison mode: run multiple times and compare means
  echo "[bench_current] Running $COMPARE_RUNS independent benchmark runs..."

  # Create temporary directory for comparison runs
  TEMP_DIR="$(mktemp -d "${OUT_JSON_ABS%.json}_compare_XXXXXX")"
  trap "rm -rf '$TEMP_DIR'" EXIT

  for run in $(seq 1 "$COMPARE_RUNS"); do
    echo "[bench_current]   Run $run/$COMPARE_RUNS (min_time=$MIN_TIME, reps=$REPS)..."
    ./test/test_benchmark \
      --benchmark_min_time="$MIN_TIME" \
      --benchmark_repetitions="$REPS" \
      --benchmark_out="$TEMP_DIR/run${run}.json" \
      --benchmark_out_format=json
  done

  # Analyze results and compare means
  echo "[bench_current] Analyzing mean differences..."
  export TEMP_DIR OUT_JSON_ABS MIN_TIME REPS
  python3 << 'PYEOF'
import json, glob, statistics, sys, os

temp_dir = os.environ.get("TEMP_DIR")
out_json = os.environ.get("OUT_JSON_ABS")

files = sorted(glob.glob(f"{temp_dir}/run*.json"))
if not files:
    print("Error: No benchmark output files found", file=sys.stderr)
    sys.exit(1)

# Extract all benchmark names from first file
with open(files[0]) as f:
    first_data = json.load(f)

benchmark_names = set()
for b in first_data['benchmarks']:
    if b.get('aggregate_name') == 'mean':
        # Extract base name without aggregate suffix
        name = b['name'].rsplit('_mean', 1)[0]
        benchmark_names.add(name)

# For each benchmark, collect means across all runs
benchmark_means = {name: [] for name in benchmark_names}

for fname in files:
    with open(fname) as f:
        data = json.load(f)

    for b in data['benchmarks']:
        if b.get('aggregate_name') == 'mean':
            name = b['name'].rsplit('_mean', 1)[0]
            if name in benchmark_names:
                benchmark_means[name].append(b['real_time'])

# Calculate statistics for each benchmark
print("\n" + "="*80)
print("MEAN COMPARISON RESULTS")
print("="*80)
print(f"Configuration: min_time={os.environ.get('MIN_TIME')}, reps={os.environ.get('REPS')}, compare_runs={len(files)}")
print()

all_cv_of_means = []
max_pairwise_diffs = []

for name in sorted(benchmark_names):
    means = benchmark_means[name]

    if len(means) < 2:
        continue

    avg_mean = statistics.mean(means)
    std_mean = statistics.stdev(means) if len(means) > 1 else 0
    cv_of_means = (std_mean / avg_mean * 100) if avg_mean > 0 else 0

    min_mean = min(means)
    max_mean = max(means)
    max_diff_pct = ((max_mean - min_mean) / avg_mean * 100) if avg_mean > 0 else 0

    all_cv_of_means.append(cv_of_means)
    max_pairwise_diffs.append(max_diff_pct)

    print(f"{name}:")
    print(f"  Average mean:     {avg_mean:>12.2f} ns")
    print(f"  Std dev of means: {std_mean:>12.2f} ns")
    print(f"  CV of means:      {cv_of_means:>11.2f}%")
    print(f"  Max pairwise diff:{max_diff_pct:>11.2f}%")
    print()

# Overall statistics
if all_cv_of_means:
    avg_cv = statistics.mean(all_cv_of_means)
    avg_max_diff = statistics.mean(max_pairwise_diffs)

    print("="*80)
    print("OVERALL STATISTICS")
    print("="*80)
    print(f"Average CV of means:      {avg_cv:.2f}%")
    print(f"Average max pairwise diff: {avg_max_diff:.2f}%")
    print()

    # Success criteria
    success = True
    if avg_cv < 3.0:
        print("✓ PASS: Average CV of means < 3%")
    else:
        print("✗ FAIL: Average CV of means >= 3%")
        success = False

    if avg_max_diff < 5.0:
        print("✓ PASS: Can detect regressions > 5%")
    else:
        print("⚠ WARNING: Max pairwise diff >= 5% (may miss small regressions)")

    print("="*80)

    if not success:
        sys.exit(1)

# Copy last run to output file
import shutil
shutil.copy(files[-1], out_json)
PYEOF

  echo "[bench_current] Done: $OUT_JSON_ABS (comparison mode)"
else
  # Normal mode: single run
  ./test/test_benchmark \
    --benchmark_min_time="$MIN_TIME" \
    --benchmark_repetitions="$REPS" \
    --benchmark_out="$OUT_JSON_ABS" \
    --benchmark_out_format=json

  echo "[bench_current] Done: $OUT_JSON_ABS"
fi
