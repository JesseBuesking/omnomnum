#!/bin/bash

echo "RUNTIME vs RELIABILITY ANALYSIS"
echo "========================================================================"
echo ""
echo "Measuring actual benchmark runtime with different settings:"
echo ""

printf "%-20s | %-10s | %-15s | %-10s\n" "Config" "Runtime" "Failure Rate" "Status"
echo "------------------------------------------------------------------------"

# Test configurations: REPS:MIN_TIME
CONFIGS=(
  "3:0.01"
  "10:0.01"
  "20:0.01"
  "30:0.01"
  "10:0.5"
)

for config in "${CONFIGS[@]}"; do
  REPS=$(echo $config | cut -d: -f1)
  MIN_TIME=$(echo $config | cut -d: -f2)

  # Compile with this REPS
  sed -i "s/const int REPETITIONS = [0-9]*/const int REPETITIONS = $REPS/" test/test_benchmark.c
  rm -f test/test_benchmark.o
  BENCH_PREFIX=/home/user/omnomnum/vendor QUICK_BENCH=1 make test/test_benchmark >/dev/null 2>&1

  # Measure runtime
  START=$(date +%s.%N)
  ./test/test_benchmark --benchmark_min_time=${MIN_TIME}s >/dev/null 2>&1
  END=$(date +%s.%N)
  RUNTIME=$(echo "$END - $START" | bc)

  # Quick failure rate (5 runs)
  FAILURES=0
  for run in {1..5}; do
    CV=$(./test/test_benchmark --benchmark_min_time=${MIN_TIME}s --benchmark_filter="BM_simple" 2>&1 | grep "BM_simple.*_cv" | awk '{print $2}')
    if [[ -n "$CV" ]] && (( $(echo "$CV >= 3.6" | bc -l) )); then
      ((FAILURES++))
    fi
  done

  FAIL_RATE=$((FAILURES * 20))

  if (( FAIL_RATE > 10 )); then
    STATUS="✗ UNRELIABLE"
  elif (( FAIL_RATE > 5 )); then
    STATUS="⚠️  MARGINAL"
  else
    STATUS="✓ RELIABLE"
  fi

  printf "reps=%2d, min=%4ss | %7.1fs | %d/5 (%2d%%)      | %s\n" \
    $REPS $MIN_TIME $RUNTIME $FAILURES $FAIL_RATE "$STATUS"
done

echo "========================================================================"
