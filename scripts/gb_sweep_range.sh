#!/usr/bin/env bash
set -euo pipefail

# Sweep benchmarks across a commit range using git worktrees.
# Writes JSON results to OUTDIR/<index>-<sha>.json
#
# Usage:
#   bash scripts/gb_sweep_range.sh <OLD_REF> [NEW_REF=HEAD] [OUTDIR=bench_runs]

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <OLD_REF> [NEW_REF] [OUTDIR]" >&2
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

OLD_REF="$1"; shift || true
NEW_REF="${1:-HEAD}"; shift || true
OUTDIR="${1:-bench_runs}"

mkdir -p "$OUTDIR"

to_abs() { case "$1" in /*) printf "%s" "$1";; *) printf "%s/%s" "$ROOT_DIR" "$1";; esac; }
OUTDIR_ABS="$(to_abs "$OUTDIR")"

echo "[sweep] Repo:   $ROOT_DIR"
echo "[sweep] Range:  $OLD_REF..$NEW_REF"
echo "[sweep] Outdir: $OUTDIR_ABS"

if command -v brew >/dev/null 2>&1; then
  export BENCH_PREFIX="${BENCH_PREFIX:-$(brew --prefix google-benchmark 2>/dev/null || true)}"
  export YAML_PREFIX="${YAML_PREFIX:-$(brew --prefix yaml-cpp 2>/dev/null || true)}"
fi
export LEMON="${LEMON:-$(command -v lemon 2>/dev/null || true)}"

# Enumerate ancestry path inclusive of both endpoints, newest last
COMMITS=$(git rev-list --ancestry-path --reverse "$OLD_REF^..$NEW_REF")
idx=0
for sha in $COMMITS; do
  idx=$((idx+1))
  short=$(git rev-parse --short=12 "$sha")
  wt="worktrees/bench-$short"
  json="$OUTDIR_ABS/$(printf "%03d" $idx)-$sha.json"
  echo "[sweep] $sha -> $json"
  git worktree add --force --detach "$wt" "$sha" >/dev/null
  (
    cd "$wt"
    make clean >/dev/null 2>&1 || true
    if [[ -x scripts/benchmark_current.sh ]]; then
      bash scripts/benchmark_current.sh "$json" || true
    else
      if make -n test/test_benchmark >/dev/null 2>&1; then
        make -j2 test/test_benchmark || true
      else
        make -j2 benchmark || true
        [[ -f test/benchmark.json ]] && cp -f test/benchmark.json "$json" || true
      fi
      if [[ -x test/test_benchmark ]]; then
        ./test/test_benchmark \
          --benchmark_min_time=2s \
          --benchmark_repetitions=3 \
          --benchmark_out="$json" \
          --benchmark_out_format=json || true
      fi
    fi
  )
done

echo "[sweep] Done. Results in $OUTDIR_ABS"

