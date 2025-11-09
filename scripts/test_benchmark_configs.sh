#!/usr/bin/env bash
set -euo pipefail

# Test various benchmark configurations to find optimal settings
# Runs meta-tests (benchmark the benchmarks) to measure:
# - CV of means: how stable are the mean times across runs?
# - Runtime: how long does a full benchmark take?
# - Regression detection: what size regression can we reliably detect?
#
# Goal: Find configuration where CV of means < 3% and runtime < 5s

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

echo "=================================="
echo "BENCHMARK CONFIGURATION META-TEST"
echo "=================================="
echo

# Configurations to test (min_time, reps)
# Start with fast configs, then test more thorough ones for baseline comparison
CONFIGS=(
  "0.05 3"
  "0.05 5"
  "0.05 10"
  "0.1 3"
  "0.1 5"
  "0.1 10"
  "0.2 3"
  "0.2 10"
  "0.5 5"
)

# Number of comparison runs for each configuration
COMPARE_RUNS=5

# Results directory
RESULTS_DIR="test/meta_results"
mkdir -p "$RESULTS_DIR"

# Summary file
SUMMARY_FILE="$RESULTS_DIR/summary.txt"
> "$SUMMARY_FILE"

echo "Testing ${#CONFIGS[@]} configurations with $COMPARE_RUNS comparison runs each"
echo "Results will be saved to: $RESULTS_DIR"
echo

for config in "${CONFIGS[@]}"; do
  read -r min_time reps <<< "$config"

  config_name="t${min_time}_r${reps}"
  echo "================================================================"
  echo "Testing configuration: min_time=${min_time}s, reps=${reps}"
  echo "================================================================"

  # Measure total runtime
  start_time=$(date +%s)

  # Run comparison test (allow failure to continue testing other configs)
  set +e
  BENCH_MIN_TIME="${min_time}s" \
  BENCH_REPS="$reps" \
  COMPARE_MODE=1 \
  COMPARE_RUNS="$COMPARE_RUNS" \
  bash scripts/benchmark_current.sh "$RESULTS_DIR/${config_name}.json" \
    > "$RESULTS_DIR/${config_name}.log" 2>&1

  exit_code=$?
  set -e
  end_time=$(date +%s)
  runtime=$((end_time - start_time))

  # Extract results from log
  if [[ -f "$RESULTS_DIR/${config_name}.log" ]]; then
    cv_of_means=$(grep "Average CV of means:" "$RESULTS_DIR/${config_name}.log" | awk '{print $5}' | sed 's/%//')
    max_diff=$(grep "Average max pairwise diff:" "$RESULTS_DIR/${config_name}.log" | awk '{print $5}' | sed 's/%//')

    # Determine pass/fail
    if [[ $exit_code -eq 0 ]]; then
      status="PASS"
    else
      status="FAIL"
    fi

    # Log to summary
    printf "%-15s %-10s CV=%-6s MaxDiff=%-6s Runtime=%-3ss %s\n" \
      "$config_name" \
      "$status" \
      "${cv_of_means}%" \
      "${max_diff}%" \
      "$runtime" \
      "" >> "$SUMMARY_FILE"

    echo
    echo "Results for $config_name:"
    echo "  CV of means:      ${cv_of_means}%"
    echo "  Max pairwise diff: ${max_diff}%"
    echo "  Runtime:          ${runtime}s"
    echo "  Status:           $status"
    echo
  else
    echo "ERROR: Log file not found for $config_name"
    printf "%-15s %-10s %s\n" "$config_name" "ERROR" "Log file not found" >> "$SUMMARY_FILE"
  fi
done

echo
echo "================================================================"
echo "META-TEST COMPLETE"
echo "================================================================"
echo
echo "Summary of all configurations:"
echo "------------------------------"
cat "$SUMMARY_FILE"
echo
echo "Detailed logs available in: $RESULTS_DIR/"
echo
echo "Recommendation:"
echo "  Choose configuration with:"
echo "  - CV of means < 3%"
echo "  - Runtime < 5s"
echo "  - Lowest max pairwise diff (better regression detection)"
