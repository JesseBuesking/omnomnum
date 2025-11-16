#!/usr/bin/env bash
set -euo pipefail

# Compare performance across builds using Google Benchmark (test/test_benchmark).
# Variants: baseline (no LTO), thin LTO, full LTO

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
cd "$ROOT_DIR"

if ! command -v make >/dev/null 2>&1; then
  echo "make not found" >&2
  exit 1
fi

run_bench() {
  local label="$1"; shift
  echo "==> Building $label"
  make clean >/dev/null 2>/dev/null
  # Build Google Benchmark target
  if (($# > 0)); then
    make test/test_benchmark "$@" >/dev/null 2>/dev/null || true
  else
    make test/test_benchmark >/dev/null 2>/dev/null || true
  fi
  echo "==> Running Google Benchmark for $label"
  local out="gb_${label}.json"
  set +e
  ( cd test && ./test_benchmark --benchmark_min_time=2s --benchmark_repetitions=10 --benchmark_out="$out" --benchmark_out_format=json )
  status=$?
  set -e
  if [[ $status -ne 0 || ! -s "test/$out" ]]; then
    echo "Google Benchmark failed or unavailable; falling back to bench_local for $label" >&2
    # Fallback: build and run local microbenchmarks
    if (($# > 0)); then
      make bench_local "$@" >/dev/null 2>/dev/null
    else
      make bench_local >/dev/null 2>/dev/null
    fi
    ./bench_local | tee "bench_${label}.txt"
  fi
}

extract_us_from_json() {
  # Parse JSON without jq: grab aggregate median real_time entries and convert to microseconds
  # Expect blocks with: "run_name": "BM_name/...", "aggregate_name": "median", "real_time": <number>, "time_unit": "ns|us|ms"
  awk '
    /"run_name"/ { 
      match($0, /"run_name"\s*:\s*"([^"]+)/, m); rn=m[1]; 
      sub(/\/repeats:.*/, "", rn); name=rn; 
    }
    /"aggregate_name"/ { if ($0 ~ /"median"/) median=1; else median=0; }
    /"real_time"/ { if (median) { match($0, /"real_time"\s*:\s*([^,]+)/, t); rt=t[1]; } }
    /"time_unit"/ { if (median) { match($0, /"time_unit"\s*:\s*"([^"]+)"/, u); unit=u[1]; 
        # convert to microseconds
        scale = (unit=="ns")? 0.001 : ((unit=="us")? 1 : ((unit=="ms")? 1000 : 1));
        us = rt * scale; printf "%s,%.3f\n", name, us; median=0; rt=""; unit=""; name=""; }
    }
  ' "$1" | sed -E 's#^.*/##' # trim path elements from run_name
}

compare_to_baseline() {
  local baseline_json="$1"; shift
  local variant_json="$1"; shift
  echo "Variant,Benchmark,us_per_iter,DeltaPct_vs_Baseline"
  # Build baseline map in memory (name -> us)
  while IFS=',' read -r bname bus; do
    eval "base_$(echo "$bname" | tr '[:alnum:]_-' '__-' | tr -cd '[:alnum:]_')=$bus"
  done < <(extract_us_from_json "$baseline_json")

  while IFS=',' read -r name us; do
    key=$(echo "$name" | tr '[:alnum:]_-' '__-' | tr -cd '[:alnum:]_')
    b=$(eval echo "\${base_${key}:-}")
    if [[ -n "$b" ]]; then
      delta=$(awk -v b="$b" -v v="$us" 'BEGIN { if (b>0) printf "%.2f", (b - v) / b * 100; else print "nan" }')
      echo "$(basename "$variant_json"),$name,$us,$delta%"
    else
      echo "$(basename "$variant_json"),$name,$us,N/A"
    fi
  done < <(extract_us_from_json "$variant_json")
}

# Fallback comparators for bench_local text
extract_us_from_txt() {
  sed -E 's/^([^:]+):.*\(([0-9.]+) us\/iter\).*$/\1,\2/' | grep -E '^[^,]+,[0-9]'
}

compare_txt_to_baseline() {
  local baseline_txt="$1"; shift
  local variant_txt="$1"; shift
  echo "Variant,Benchmark,us_per_iter,DeltaPct_vs_Baseline"
  while IFS=',' read -r name us; do
    b=$(grep -E "^${name}:" "$baseline_txt" | sed -E 's/.*\(([0-9.]+) us\/iter\).*/\1/' | head -n1)
    if [[ -n "$b" ]]; then
      delta=$(awk -v b="$b" -v v="$us" 'BEGIN { if (b>0) printf "%.2f", (b - v) / b * 100; else print "nan" }')
      echo "$(basename "$variant_txt"),$name,$us,$delta%"
    else
      echo "$(basename "$variant_txt"),$name,$us,N/A"
    fi
  done < <(extract_us_from_txt < "$variant_txt")
}

arch=$(uname -m)
echo "Architecture: $arch"

# Baseline (no LTO)
run_bench baseline

# Thin LTO
run_bench lto_thin USE_LTO=thin

# Full LTO
run_bench lto_full USE_LTO=full

echo
echo "=== Summary vs baseline (lower us/iter is better; positive delta means faster) ==="
if [[ -s test/gb_baseline.json && -s test/gb_lto_thin.json && -s test/gb_lto_full.json ]]; then
  compare_to_baseline test/gb_baseline.json test/gb_lto_thin.json | column -t -s','
  echo
  compare_to_baseline test/gb_baseline.json test/gb_lto_full.json | column -t -s','
  echo
  echo "Raw outputs saved to: test/gb_baseline.json, test/gb_lto_thin.json, test/gb_lto_full.json"
else
  echo "Google Benchmark output not available; showing bench_local comparison instead." >&2
  compare_txt_to_baseline bench_baseline.txt bench_lto_thin.txt | column -t -s','
  echo
  compare_txt_to_baseline bench_baseline.txt bench_lto_full.txt | column -t -s','
  echo
  echo "Raw outputs saved to: bench_baseline.txt, bench_lto_thin.txt, bench_lto_full.txt"
fi
