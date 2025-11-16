#!/usr/bin/env bash
set -euo pipefail

# Quick build test for ALL commits (master..HEAD) to verify Makefile patches work
# Usage: bash test_all_builds.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

# Detect google-benchmark
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]] && command -v brew >/dev/null 2>&1; then
  BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[test] Error: google-benchmark not found. Set BENCH_PREFIX or install via Homebrew." >&2
  exit 1
fi
echo "[test] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"

# Get ALL commits from master to HEAD
COMMITS=( )
while IFS= read -r c; do COMMITS+=("$c"); done < <(git rev-list --reverse master..HEAD)

echo "[test] Testing ${#COMMITS[@]} commits for build success"
echo ""

PASS=0
FAIL=0
FAILED_COMMITS=()

idx=0
for c in "${COMMITS[@]}"; do
  idx=$((idx+1))
  short="$(printf "%s" "$c" | cut -c1-12)"
  printf "[test] %3d/%-3d %-12s ... " "$idx" "${#COMMITS[@]}" "$short"

  WT_DIR="$(mktemp -d 2>/dev/null || mktemp -d -t build-test)"
  cleanup_one() { git worktree remove -f "$WT_DIR" >/dev/null 2>&1 || true; rm -rf "$WT_DIR" || true; }
  trap cleanup_one RETURN
  git worktree add --detach "$WT_DIR" "$c" >/dev/null 2>&1

  BUILD_SUCCESS=0
  (
    cd "$WT_DIR"

    # Copy current benchmark suite AND updated benchmark_current.sh
    mkdir -p test scripts
    if [[ -f "$ROOT_DIR/test/test_benchmark.c" ]]; then
      cp -f "$ROOT_DIR/test/test_benchmark.c" test/test_benchmark.c
    fi
    if [[ -f "$ROOT_DIR/scripts/benchmark_current.sh" ]]; then
      cp -f "$ROOT_DIR/scripts/benchmark_current.sh" scripts/benchmark_current.sh
      chmod +x scripts/benchmark_current.sh
    fi

    # Apply Makefile patches
    if [[ -f Makefile ]]; then
      if grep -q "yaml-cpp" Makefile 2>/dev/null; then
        sed -i.bak1 's/-lyaml-cpp//g' Makefile
      fi
      if grep -q "\-L/usr/local/include" Makefile 2>/dev/null; then
        sed -i.bak2 "s|-L/usr/local/include|-I${BENCH_PREFIX_ENV}/include -L${BENCH_PREFIX_ENV}/lib|g" Makefile
      fi
    fi

    # Try to build
    if make test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" >/dev/null 2>&1; then
      exit 0
    fi

    # Fix for 5 commits with misplaced object files:
    # Some old commits build grisu2.o and branchlut.o in top-level dir instead of subdirs
    mkdir -p grisu2 branchlut
    [[ -f grisu2.o && ! -f grisu2/grisu2.o ]] && mv -f grisu2.o grisu2/grisu2.o
    [[ -f branchlut.o && ! -f branchlut/branchlut.o ]] && mv -f branchlut.o branchlut/branchlut.o

    # Retry build after moving object files
    if make test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" >/dev/null 2>&1; then
      exit 0
    else
      exit 1
    fi
  ) && BUILD_SUCCESS=1 || BUILD_SUCCESS=0

  if [[ $BUILD_SUCCESS -eq 1 ]]; then
    echo "✓ PASS"
    PASS=$((PASS + 1))
  else
    echo "✗ FAIL"
    FAIL=$((FAIL + 1))
    FAILED_COMMITS+=("$short")
  fi

  cleanup_one
  trap - RETURN
done

echo ""
echo "========================================"
echo "Build Test Results (ALL COMMITS)"
echo "========================================"
echo "Passed: $PASS / ${#COMMITS[@]}"
echo "Failed: $FAIL / ${#COMMITS[@]}"

if [[ $FAIL -gt 0 ]]; then
  echo ""
  echo "Failed commits:"
  for fc in "${FAILED_COMMITS[@]}"; do
    echo "  - $fc"
  done
  echo ""
  echo "Some commits failed to build. You may need additional Makefile patches."
  exit 1
else
  echo ""
  echo "✓ All ${#COMMITS[@]} commits build successfully!"
  echo "Ready to run full benchmark sweep on all commits."
fi
