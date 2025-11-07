#!/usr/bin/env bash
set -euo pipefail

# Run a small matrix of build toggles and benchmark each configuration.
# JSONs are written under test/ with a gb-tgl-*.json prefix, and a summary CSV
# is emitted as test/gb-toggle-means.csv.
#
# Usage:
#   bash scripts/gb_toggle_sweep.sh [--freeze]
#
# Notes:
# - Uses scripts/benchmark_current.sh (hardened runner) so it should work across refs.
# - --freeze passes FREEZE_CODEGEN=1 to the runner to avoid regenerating parser/scanner
#   when the generated files exist in the ref.
# - You can override BENCH_PREFIX in the environment.

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

FREEZE_FLAG=()
if [[ ${1:-} == "--freeze" ]]; then FREEZE_FLAG=(FREEZE_CODEGEN=1); fi

# Locate google-benchmark prefix if not provided
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]] && command -v brew >/dev/null 2>&1; then
  BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[toggle] Error: google-benchmark not found. Set BENCH_PREFIX or install via Homebrew." >&2
  exit 1
fi
echo "[toggle] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"

run_case() {
  local name="$1"; shift
  local cflags="$*"
  local out="test/gb-tgl-${name}.json"
  echo "[toggle] Case: $name  CCFLAGS='$cflags'"
  CCFLAGS="$cflags" CXXFLAGS="$cflags" BENCH_PREFIX="$BENCH_PREFIX_ENV" "${FREEZE_FLAG[@]}" \
    bash scripts/benchmark_current.sh "$out" || true
}

# Cases
run_case default

# Scanner fractions OFF/ON (override any defaults)
run_case sf-off "-USCANNER_FRACTIONS -DSCANNER_FRACTIONS=0"
run_case sf-on  "-USCANNER_FRACTIONS -DSCANNER_FRACTIONS=1"

# Placeholder cases for optional perf toggles. Harmless if code doesnt reference them.
run_case reuse-parser-off "-DPERF_TOGGLE_REUSE_PARSER_OFF"
run_case direct-render-off "-DPERF_TOGGLE_DIRECT_RENDER_OFF"

echo "[toggle] Extracting means -> test/gb-toggle-means.csv"
bash scripts/gb_extract_means.sh test/gb-tgl-*.json > test/gb-toggle-means.csv || true
echo "[toggle] Done. See test/gb-toggle-means.csv"

