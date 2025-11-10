#!/usr/bin/env bash
set -euo pipefail

# Sweep a commit range, benchmark each commit in order, and emit per-commit means
# plus deltas vs the previous commit.
#
# Usage:
#   bash scripts/gb_sweep_and_diff.sh <START_REF> <END_REF> <OUT_DIR> [--freeze]
#
# Notes:
# - Builds each commit in an isolated git worktree.
# - Writes JSONs as <OUT_DIR>/<NNN>-<shortsha>.json, then composes:
#     <OUT_DIR>/means.csv               (BM_* means for each commit)
#     <OUT_DIR>/means_with_deltas.csv   (adds deltas and percents vs previous)
# - If --freeze (or env FREEZE_CODEGEN=1) is provided and the ref tracks
#   parser.c/scanner.c, the script restores and touches those generated files
#   and passes LEMON=/usr/bin/false RE2C=/usr/bin/false to make to avoid regen.
# - Falls back to /tmp for JSON writes if repo path is restricted.

if [[ $# -lt 3 ]]; then
  echo "Usage: $0 <START_REF> <END_REF> <OUT_DIR> [--freeze] [--desc]" >&2
  exit 1
fi

START_REF="$1"; shift
END_REF="$1"; shift
OUT_DIR="$1"; shift

FREEZE_CODEGEN="${FREEZE_CODEGEN:-0}"
ORDER_DESC=0
for arg in "$@"; do
  case "$arg" in
    --freeze) FREEZE_CODEGEN=1 ;;
    --desc) ORDER_DESC=1 ;;
  esac
done

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

# Resolve out dir absolute path
case "$OUT_DIR" in
  /*) OUT_ABS="$OUT_DIR" ;;
   *) OUT_ABS="$ROOT_DIR/$OUT_DIR" ;;
esac
mkdir -p "$OUT_ABS"

# Detect google-benchmark
BENCH_PREFIX_ENV="${BENCH_PREFIX:-}"
if [[ -z "$BENCH_PREFIX_ENV" ]] && command -v brew >/dev/null 2>&1; then
  BENCH_PREFIX_ENV="$(brew --prefix google-benchmark 2>/dev/null || true)"
fi
if [[ -z "$BENCH_PREFIX_ENV" ]]; then
  echo "[sweep] Error: google-benchmark not found. Set BENCH_PREFIX or install via Homebrew." >&2
  exit 1
fi
echo "[sweep] Using BENCH_PREFIX=$BENCH_PREFIX_ENV"

# Build commit list (inclusive range, ordered oldest->newest)
# Build commit list
COMMITS=( )
if [[ "$ORDER_DESC" == "1" ]]; then
  # Newer → older: prefer END..START (newest-first); fall back to swapped.
  while IFS= read -r c; do COMMITS+=("$c"); done < <(git rev-list "${END_REF}^..${START_REF}")
  if [[ ${#COMMITS[@]} -eq 0 ]]; then
    echo "[sweep] Empty range ${END_REF}..${START_REF}; trying swapped order (desc)" >&2
    while IFS= read -r c; do COMMITS+=("$c"); done < <(git rev-list "${START_REF}^..${END_REF}")
  fi
else
  # Oldest → newest: prefer START..END (oldest-first);
  while IFS= read -r c; do COMMITS+=("$c"); done < <(git rev-list --reverse "${START_REF}^..${END_REF}")
  if [[ ${#COMMITS[@]} -eq 0 ]]; then
    echo "[sweep] Empty range ${START_REF}..${END_REF}; trying swapped order (asc)" >&2
    while IFS= read -r c; do COMMITS+=("$c"); done < <(git rev-list --reverse "${END_REF}^..${START_REF}")
  fi
fi
if [[ ${#COMMITS[@]} -eq 0 ]]; then
  echo "[sweep] No commits in either order: ${START_REF}..${END_REF}" >&2
  exit 1
fi
echo "[sweep] Commits: ${#COMMITS[@]} from $(echo ${COMMITS[0]} | cut -c1-12) to $(echo ${COMMITS[-1]} | cut -c1-12)"

idx=0
for c in "${COMMITS[@]}"; do
  idx=$((idx+1))
  short="$(printf "%s" "$c" | cut -c1-12)"
  json="$OUT_ABS/$(printf "%03d" "$idx")-$short.json"
  echo "[sweep] Bench $short -> $json"

  WT_DIR="$(mktemp -d 2>/dev/null || mktemp -d -t gb-sweep)"
  cleanup_one() { git worktree remove -f "$WT_DIR" >/dev/null 2>&1 || true; rm -rf "$WT_DIR" || true; }
  trap cleanup_one RETURN
  git worktree add --detach "$WT_DIR" "$c" >/dev/null

  (
    cd "$WT_DIR"
    # Copy hardened runner into worktree so older commits use the updated logic
    mkdir -p scripts
    cp -f "$ROOT_DIR/scripts/benchmark_current.sh" scripts/benchmark_current.sh
    chmod +x scripts/benchmark_current.sh

    # Copy current benchmark suite to test all commits with same comprehensive benchmarks
    mkdir -p test
    if [[ -f "$ROOT_DIR/test/test_benchmark.c" ]]; then
      cp -f "$ROOT_DIR/test/test_benchmark.c" test/test_benchmark.c
      echo "[sweep] Copied current test_benchmark.c to $short"
    fi

    # Optional freeze of generated files if tracked in this ref (handled by runner via FREEZE_CODEGEN)
    FREEZE_VARS=()
    if [[ "$FREEZE_CODEGEN" == "1" ]]; then FREEZE_VARS=(FREEZE_CODEGEN=1); fi

    # Try hardened runner; fall back to /tmp if repo path is restricted
    if ! env BENCH_PREFIX="$BENCH_PREFIX_ENV" "${FREEZE_VARS[@]}" bash scripts/benchmark_current.sh "$json"; then
      echo "[sweep] benchmark_current.sh failed; attempting object repair and retry"
    fi
    if [[ ! -s "$json" ]]; then
      tmpj="/tmp/$(basename "$json")"
      env BENCH_PREFIX="$BENCH_PREFIX_ENV" "${FREEZE_VARS[@]}" bash scripts/benchmark_current.sh "$tmpj" || true
      if [[ -s "$tmpj" ]]; then cp -f "$tmpj" "$json" 2>/dev/null || true; fi
    fi

    if [[ ! -s "$json" ]]; then
      echo "[sweep] ERROR: no JSON for $short" >&2
    fi
  )

  cleanup_one
  trap - RETURN
done

# Compose means CSV in the observed order
MEANS="$OUT_ABS/means.csv"
echo "[sweep] Extracting means -> $MEANS"
files=( $(ls -1 "$OUT_ABS"/*.json | sort) )
bash scripts/gb_extract_means.sh "${files[@]}" > "$MEANS"

# Add deltas vs previous commit
DELTA_OUT="$OUT_ABS/means_with_deltas.csv"
python3 - "$MEANS" "$DELTA_OUT" <<'PYTHON'
import sys
import csv

means_file = sys.argv[1]
delta_file = sys.argv[2]

# Read all rows
with open(means_file) as f:
    reader = csv.DictReader(f)
    rows = list(reader)
    fieldnames = reader.fieldnames

# Get benchmark columns (all except 'file')
bench_cols = [col for col in fieldnames if col != 'file']

# Build output headers: file, bench1, bench2, ..., bench1_delta, bench1_pct, bench2_delta, bench2_pct, ...
output_headers = ['file']
for col in bench_cols:
    output_headers.append(col)
for col in bench_cols:
    output_headers.append(f'{col}_delta')
    output_headers.append(f'{col}_pct')

# Write output with deltas
with open(delta_file, 'w') as f:
    writer = csv.writer(f)
    writer.writerow(output_headers)

    prev_vals = None
    for i, row in enumerate(rows):
        output_row = [row['file']]

        # Add benchmark values (handle missing/empty values as None)
        curr_vals = []
        for col in bench_cols:
            val = row[col].strip()
            curr_vals.append(float(val) if val else None)

        # Format values for output (None -> empty string)
        output_row.extend([f'{v:.2f}' if v is not None else '' for v in curr_vals])

        # Add deltas and percentages
        if prev_vals is None:
            # First row: all deltas are 0 or empty
            for v in curr_vals:
                output_row.extend(['0.00' if v is not None else '', '0.00' if v is not None else ''])
        else:
            for prev, curr in zip(prev_vals, curr_vals):
                if curr is None or prev is None:
                    # Missing benchmark: leave delta empty
                    output_row.extend(['', ''])
                else:
                    delta = curr - prev
                    pct = (100.0 * delta / prev) if prev != 0 else 0
                    output_row.extend([f'{delta:.2f}', f'{pct:+.2f}'])

        writer.writerow(output_row)
        prev_vals = curr_vals
PYTHON

echo "[sweep] Done. Outputs:"
echo "  JSONs: $OUT_ABS/*.json"
echo "  Means: $MEANS"
echo "  Deltas: $DELTA_OUT"
