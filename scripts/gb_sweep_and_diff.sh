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
awk -F',' 'BEGIN{OFS=","}
  NR==1{print $0, "d_simple_ns","d_simple_pct","d_long_ns","d_long_pct","d_many_ns","d_many_pct"; next}
  NR==2{print $0, 0, 0, 0, 0, 0, 0; ps=$2; pl=$3; pm=$4; next}
  {
    ds=$2-ps; dls=$3-pl; dm=$4-pm;
    pcts=(ps!=0?100.0*ds/ps:0); pctl=(pl!=0?100.0*dls/pl:0); pctm=(pm!=0?100.0*dm/pm:0);
    print $0, ds, pcts, dls, pctl, dm, pctm;
    ps=$2; pl=$3; pm=$4;
  }
' "$MEANS" > "$DELTA_OUT"

echo "[sweep] Done. Outputs:"
echo "  JSONs: $OUT_ABS/*.json"
echo "  Means: $MEANS"
echo "  Deltas: $DELTA_OUT"
