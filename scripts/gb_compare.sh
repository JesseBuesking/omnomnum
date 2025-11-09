#!/usr/bin/env bash
set -euo pipefail

# Google Benchmark comparison for OmNomNum.
# - Stashes current changes to build/run OLD (HEAD) code
# - Builds and runs Google Benchmark, saving JSON as old
# - Restores changes to build/run NEW (working tree) code
# - Builds and runs Google Benchmark again, saving JSON as new
#
# Usage:
#   bash scripts/gb_compare.sh [old_json_path] [new_json_path]
#
# Defaults (timestamped files under test/):
#   test/gb-old-<timestamp>.json
#   test/gb-new-<timestamp>.json

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

timestamp="$(date +%Y%m%d-%H%M%S)"
OLD_JSON="${1:-test/gb-old-${timestamp}.json}"
NEW_JSON="${2:-test/gb-new-${timestamp}.json}"

echo "[gb_compare] Repo:        $ROOT_DIR"
echo "[gb_compare] Old JSON:    $OLD_JSON"
echo "[gb_compare] New JSON:    $NEW_JSON"

stash_ref=""

restore_stash() {
  if [[ -n "$stash_ref" ]]; then
    echo "[gb_compare] Restoring working changes from stash $stash_ref ..."
    # Use 'git stash pop' to apply and drop the stash. If conflicts, leave as-is for user to resolve.
    if ! git stash pop -q "$stash_ref"; then
      echo "[gb_compare] Warning: git stash pop reported conflicts. Please resolve them manually."
    fi
    stash_ref=""
  fi
}
trap restore_stash EXIT

has_changes() {
  [[ -n "$(git status --porcelain)" ]]
}

build_bench() {
  echo "[gb_compare] Building Google Benchmark harness ..."
  make clean >/dev/null 2>&1 || true
  make test/test_benchmark
}

run_bench_to_json() {
  local out_json="$1"
  mkdir -p "$(dirname "$out_json")"
  echo "[gb_compare] Running benchmarks -> $out_json ..."
  ./test/test_benchmark \
    --benchmark_min_time=0.5s \
    --benchmark_repetitions=3 \
    --benchmark_out="$out_json" \
    --benchmark_out_format=json
}

echo "[gb_compare] Detecting working changes ..."
if has_changes; then
  echo "[gb_compare] Stashing current changes ..."
  git stash push -u -m "gb-compare-$timestamp" >/dev/null
  stash_ref="$(git stash list | head -n 1 | cut -d: -f1)"
  echo "[gb_compare] Stashed as $stash_ref"
else
  echo "[gb_compare] No uncommitted changes. Using current HEAD as OLD baseline."
fi

# Build and run OLD (HEAD) code
build_bench
run_bench_to_json "$OLD_JSON"

# Ensure tree is clean before restoring NEW changes to avoid merge conflicts
echo "[gb_compare] Resetting tree to clean HEAD before restoring changes ..."
git reset --hard -q HEAD

# Restore NEW changes
if [[ -n "$stash_ref" ]]; then
  restore_stash
fi

# Build and run NEW (working tree) code
build_bench
run_bench_to_json "$NEW_JSON"

echo "[gb_compare] Done. Results:"
echo "  OLD -> $OLD_JSON"
echo "  NEW -> $NEW_JSON"
