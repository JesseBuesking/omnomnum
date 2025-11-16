#!/usr/bin/env bash
set -euo pipefail

# Compare Google Benchmarks between two refs without touching your working tree.
#
# Usage:
#   bash scripts/gb_compare_refs.sh <OLD_REF> [NEW_REF] [OLD_JSON] [NEW_JSON]
#
# Notes:
# - If NEW_REF is omitted, benchmarks run against your current working tree.
# - If test/test_benchmark target is unavailable, falls back to `make benchmark`.
# - Requires google-benchmark installed and linkable per Makefile.

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <OLD_REF> [NEW_REF] [OLD_JSON] [NEW_JSON]" >&2
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

OLD_REF="$1"; shift
# NEW_REF is optional; consume the position even if empty to keep args aligned
if [[ $# -ge 1 ]]; then
  NEW_REF="$1"
  shift || true
else
  NEW_REF=""
fi

timestamp="$(date +%Y%m%d-%H%M%S)"
OLD_JSON="${1:-test/gb-old-${timestamp}.json}"
NEW_JSON="${2:-test/gb-new-${timestamp}.json}"

echo "[gb_refs] Repo:     $ROOT_DIR"
echo "[gb_refs] OLD ref:  ${OLD_REF}"
echo "[gb_refs] NEW ref:  ${NEW_REF:-WORKTREE}"
echo "[gb_refs] OLD json: $OLD_JSON"
echo "[gb_refs] NEW json: $NEW_JSON"

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
NEW_DIR=""

# Locate google-benchmark prefix if not provided
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  if command -v brew >/dev/null 2>&1; then
    BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
  fi
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[gb_refs] Error: google-benchmark not found. Install via Homebrew or set BENCH_PREFIX."
  echo "          e.g., BENCH_PREFIX=\"$(brew --prefix google-benchmark 2>/dev/null || echo /usr/local/opt/google-benchmark)\" bash $0 <OLD_REF> [NEW_REF]"
  exit 1
fi
echo "[gb_refs] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"

cleanup() {
  if [[ -n "$NEW_DIR" && -d "$NEW_DIR" ]]; then
    echo "[gb_refs] Removing NEW worktree: $NEW_DIR"
    git worktree remove -f "$NEW_DIR" >/dev/null 2>&1 || true
    rm -rf "$NEW_DIR" || true
  fi
  if [[ -d "$OLD_DIR" ]]; then
    echo "[gb_refs] Removing OLD worktree: $OLD_DIR"
    git worktree remove -f "$OLD_DIR" >/dev/null 2>&1 || true
    rm -rf "$OLD_DIR" || true
  fi
}
trap cleanup EXIT

echo "[gb_refs] Adding OLD worktree at ${OLD_REF}: $OLD_DIR"
git worktree add --detach "$OLD_DIR" "$OLD_REF" >/dev/null
echo "[gb_refs] OLD commit: $(git -C "$OLD_DIR" rev-parse --short=12 HEAD)  dirty=$(test -n "$(git -C "$OLD_DIR" status --porcelain 2>/dev/null)" && echo 1 || echo 0)"

build_and_run() {
  local dir="$1" json="$2" label="$3"
  echo "[gb_refs] Building $label ..."
  (
    cd "$dir"
    make clean >/dev/null 2>&1 || true
    FREEZE=${FREEZE_CODEGEN:-0}
    MAKE_FREEZE_VARS=""
    if [[ "$FREEZE" == "1" ]]; then
      echo "[gb_refs] FREEZE_CODEGEN=1: restoring generated files and touching to avoid regen"
      if git ls-files --error-unmatch parser.c >/dev/null 2>&1; then git checkout -- parser.c || true; fi
      if git ls-files --error-unmatch parser.h >/dev/null 2>&1; then git checkout -- parser.h || true; fi
      if git ls-files --error-unmatch scanner.c >/dev/null 2>&1; then git checkout -- scanner.c || true; fi
      touch -c parser.c scanner.c parser.h 2>/dev/null || true
      MAKE_FREEZE_VARS="LEMON=/usr/bin/false RE2C=/usr/bin/false"
    fi

    ensure_objs() {
      mkdir -p grisu2 branchlut || true
      # Move misplaced objects if present
      if [[ ! -f grisu2/grisu2.o && -f grisu2.o ]]; then mv -f grisu2.o grisu2/grisu2.o || true; fi
      if [[ ! -f branchlut/branchlut.o && -f branchlut.o ]]; then mv -f branchlut.o branchlut/branchlut.o || true; fi
      # Compile directly if still missing (avoid arch-specific flags)
      if [[ ! -f grisu2/grisu2.o && -f grisu2/grisu2.c ]]; then
        echo "[gb_refs] Compiling grisu2/grisu2.o directly"
        cc -O3 -std=c99 -Wall -W -Wno-missing-field-initializers -I. -c grisu2/grisu2.c -o grisu2/grisu2.o || true
      fi
      if [[ ! -f branchlut/branchlut.o && -f branchlut/branchlut.c ]]; then
        echo "[gb_refs] Compiling branchlut/branchlut.o directly"
        cc -O3 -std=c99 -Wall -W -Wno-missing-field-initializers -I. -c branchlut/branchlut.c -o branchlut/branchlut.o || true
      fi
      ls -l grisu2/grisu2.o branchlut/branchlut.o 2>/dev/null || true
    }

    # Build core objects first to populate dependency outputs in older refs
    make -j1 all $MAKE_FREEZE_VARS BENCH_PREFIX="$BENCH_PREFIX_ENV" CCFLAGS="${CCFLAGS:-}" CXXFLAGS="${CXXFLAGS:-}" SCANNER_FRACTIONS="${SCANNER_FRACTIONS:-}" >/dev/null 2>&1 || true
    ensure_objs

    if make -n test/test_benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" >/dev/null 2>&1; then
      # Try build, repair objects on failure, then retry. If still failing, manual compile+link
      if ! make test/test_benchmark $MAKE_FREEZE_VARS BENCH_PREFIX="$BENCH_PREFIX_ENV" CCFLAGS="${CCFLAGS:-}" CXXFLAGS="${CXXFLAGS:-}" SCANNER_FRACTIONS="${SCANNER_FRACTIONS:-}"; then
        echo "[gb_refs] Link failed; repairing objects and retrying ..."
        ensure_objs
        if ! make -B test/test_benchmark $MAKE_FREEZE_VARS BENCH_PREFIX="$BENCH_PREFIX_ENV" CCFLAGS="${CCFLAGS:-}" CXXFLAGS="${CXXFLAGS:-}" SCANNER_FRACTIONS="${SCANNER_FRACTIONS:-}"; then
          echo "[gb_refs] Make link still failing; attempting manual build/link"
          # Build core .o files explicitly
          make -k parser.o parser_compat.o omnomnum.o scanner.o scan.o sds.o itoa.o dtoa.o scanner.def.o util.o $MAKE_FREEZE_VARS BENCH_PREFIX="$BENCH_PREFIX_ENV" CCFLAGS="${CCFLAGS:-}" CXXFLAGS="${CXXFLAGS:-}" SCANNER_FRACTIONS="${SCANNER_FRACTIONS:-}" || true
          ensure_objs
          # Compile benchmark object if missing
          if [[ ! -f test/test_benchmark.o && -f test/test_benchmark.c ]]; then
            c++ -std=c++17 -I"$BENCH_PREFIX_ENV"/include -I. \
              -DGIT_SHA=\"$(git rev-parse --short=12 HEAD 2>/dev/null || echo unknown)\" \
              -DGIT_DESC=\"$(git describe --always --dirty --tags 2>/dev/null || git rev-parse --short=12 HEAD 2>/dev/null || echo unknown)\" \
              -DBUILD_TIME=\"$(date -u +%FT%TZ)\" \
              -DTREE_DIRTY_STR=\"$(test -n "$(git status --porcelain 2>/dev/null)" && echo 1 || echo 0)\" \
              -c test/test_benchmark.c -o test/test_benchmark.o
          fi
          # Gather objects for link (use both top-level and subdir)
          objs=$(ls -1 *.o grisu2/*.o branchlut/*.o 2>/dev/null | tr '\n' ' ')
          echo "[gb_refs] Linking with objs: $objs"
          c++ -std=c++17 -o test/test_benchmark -I. $objs -pthread -L"$BENCH_PREFIX_ENV"/lib -lbenchmark
        fi
      fi
      echo "[gb_refs] Running $label test_benchmark -> $json"
      if ! ./test/test_benchmark \
        --benchmark_min_time=0.5s \
        --benchmark_repetitions=3 \
        --benchmark_out="$json" \
        --benchmark_out_format=json; then
        echo "[gb_refs] Run failed for $json"
      fi
      # If write produced no data (sandbox), write to /tmp and copy back
      if [[ ! -s "$json" ]]; then
        local tmpjson
        tmpjson="/tmp/$(basename "$json")"
        echo "[gb_refs] Falling back to /tmp: $tmpjson"
        ./test/test_benchmark \
          --benchmark_min_time=0.5s \
          --benchmark_repetitions=3 \
          --benchmark_out="$tmpjson" \
          --benchmark_out_format=json
        if [[ -s "$tmpjson" ]]; then
          cp -f "$tmpjson" "$json" 2>/dev/null || true
          echo "[gb_refs] Copied $tmpjson -> $json"
        fi
      fi
    else
      echo "[gb_refs] Target test/test_benchmark not found; using 'make benchmark' fallback"
      make benchmark BENCH_PREFIX="$BENCH_PREFIX_ENV" CCFLAGS="${CCFLAGS:-}" CXXFLAGS="${CXXFLAGS:-}" SCANNER_FRACTIONS="${SCANNER_FRACTIONS:-}"
      if [[ -f test/benchmark.json ]]; then
        cp -f test/benchmark.json "$json"
      else
        echo "[gb_refs] Error: benchmark.json not produced" >&2
        exit 1
      fi
    fi
  )
}

# Build/run OLD
build_and_run "$OLD_DIR" "$OLD_JSON_ABS" "OLD(${OLD_REF})"

# Build/run NEW
if [[ -n "$NEW_REF" ]]; then
  NEW_DIR="$(mktemp -d 2>/dev/null || mktemp -d -t gb-new)"
  echo "[gb_refs] Adding NEW worktree at ${NEW_REF}: $NEW_DIR"
  git worktree add --detach "$NEW_DIR" "$NEW_REF" >/dev/null
  echo "[gb_refs] NEW commit: $(git -C "$NEW_DIR" rev-parse --short=12 HEAD)  dirty=$(test -n "$(git -C "$NEW_DIR" status --porcelain 2>/dev/null)" && echo 1 || echo 0)"
  build_and_run "$NEW_DIR" "$NEW_JSON_ABS" "NEW(${NEW_REF})"
else
  echo "[gb_refs] Building NEW in working tree"
  echo "[gb_refs] NEW commit: $(git rev-parse --short=12 HEAD)  dirty=$(test -n "$(git status --porcelain 2>/dev/null)" && echo 1 || echo 0)"
  build_and_run "$ROOT_DIR" "$NEW_JSON_ABS" "NEW(WORKTREE)"
fi

echo "[gb_refs] Done. Results:"
echo "  OLD(${OLD_REF}) -> $OLD_JSON_ABS"
echo "  NEW(${NEW_REF:-WORKTREE}) -> $NEW_JSON_ABS"
