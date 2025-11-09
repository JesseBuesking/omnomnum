#!/usr/bin/env bash
set -euo pipefail

# Google Benchmark comparison without touching your working tree.
# Uses a temporary git worktree for OLD (HEAD) build, then builds NEW in cwd.
#
# Usage:
#   bash scripts/gb_compare_worktree.sh [old_json_path] [new_json_path]
#
# Defaults (timestamped under test/):
#   test/gb-old-<timestamp>.json
#   test/gb-new-<timestamp>.json

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

timestamp="$(date +%Y%m%d-%H%M%S)"
OLD_JSON="${1:-test/gb-old-${timestamp}.json}"
NEW_JSON="${2:-test/gb-new-${timestamp}.json}"

echo "[gb_worktree] Repo:     $ROOT_DIR"
echo "[gb_worktree] OLD json: $OLD_JSON"
echo "[gb_worktree] NEW json: $NEW_JSON"

# Compute absolute output paths so results survive worktree cleanup
to_abs_path() {
  case "$1" in
    /*) printf "%s" "$1" ;;
     *) printf "%s/%s" "$ROOT_DIR" "$1" ;;
  esac
}
OLD_JSON_ABS="$(to_abs_path "$OLD_JSON")"
NEW_JSON_ABS="$(to_abs_path "$NEW_JSON")"

mkdir -p "$(dirname "$OLD_JSON_ABS")" "$(dirname "$NEW_JSON_ABS")"

OLD_DIR="$(mktemp -d 2>/dev/null || mktemp -d -t gb-old)"
# Locate google-benchmark prefix if not provided
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  if command -v brew >/dev/null 2>&1; then
    BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
  fi
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[gb_worktree] Error: google-benchmark not found. Install via Homebrew or set BENCH_PREFIX."
  echo "               e.g., BENCH_PREFIX=\"$(brew --prefix google-benchmark 2>/dev/null || echo /usr/local/opt/google-benchmark)\" bash $0"
  exit 1
fi
echo "[gb_worktree] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"
cleanup() {
  if [[ -d "$OLD_DIR" ]]; then
    echo "[gb_worktree] Removing worktree: $OLD_DIR"
    # Try to remove via git worktree; ignore errors if already gone
    git worktree remove -f "$OLD_DIR" >/dev/null 2>&1 || true
    rm -rf "$OLD_DIR" || true
  fi
}
trap cleanup EXIT

echo "[gb_worktree] Adding worktree at HEAD: $OLD_DIR"
git worktree add --detach "$OLD_DIR" HEAD >/dev/null
echo "[gb_worktree] OLD commit: $(git -C "$OLD_DIR" rev-parse --short=12 HEAD)  dirty=$(test -n "$(git -C "$OLD_DIR" status --porcelain 2>/dev/null)" && echo 1 || echo 0)"

# Build and run OLD in the worktree
echo "[gb_worktree] Building OLD benchmark ..."
(
  cd "$OLD_DIR"
  make clean >/dev/null 2>&1 || true
  if make -n test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" >/dev/null 2>&1; then
    make test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV"
    echo "[gb_worktree] Running OLD test_benchmark -> $OLD_JSON_ABS"
    ./test/test_benchmark \
      --benchmark_min_time=2s \
      --benchmark_repetitions=3 \
      --benchmark_out="$OLD_JSON_ABS" \
      --benchmark_out_format=json
  else
    echo "[gb_worktree] Target test/test_benchmark not found; using 'make benchmark' fallback"
    make benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV"
    if [[ -f test/benchmark.json ]]; then
      cp -f test/benchmark.json "$OLD_JSON_ABS"
    else
      echo "[gb_worktree] Error: benchmark.json not produced" >&2
      exit 1
    fi
  fi
)

# Build and run NEW in current working tree
echo "[gb_worktree] Building NEW benchmark ..."
make clean >/dev/null 2>&1 || true
if make -n test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" >/dev/null 2>&1; then
  make test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV"
  echo "[gb_worktree] Running NEW test_benchmark -> $NEW_JSON_ABS"
  ./test/test_benchmark \
    --benchmark_min_time=2s \
    --benchmark_repetitions=3 \
    --benchmark_out="$NEW_JSON_ABS" \
    --benchmark_out_format=json
else
  echo "[gb_worktree] Target test/test_benchmark not found; using 'make benchmark' fallback"
  make benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV"
  if [[ -f test/benchmark.json ]]; then
    cp -f test/benchmark.json "$NEW_JSON_ABS"
  else
    echo "[gb_worktree] Error: benchmark.json not produced" >&2
    exit 1
  fi
fi

echo "[gb_worktree] NEW commit: $(git rev-parse --short=12 HEAD)  dirty=$(test -n "$(git status --porcelain 2>/dev/null)" && echo 1 || echo 0)"

echo "[gb_worktree] Done. Results:"
echo "  OLD -> $OLD_JSON_ABS"
echo "  NEW -> $NEW_JSON_ABS"
