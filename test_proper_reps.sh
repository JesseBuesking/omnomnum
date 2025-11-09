#!/bin/bash

echo "PROPER TEST: Varying REPS with all 25 benchmarks"
echo "================================================================"
echo ""

echo "Test 1: Varying REPS (min_time=0.01s constant)"
echo "----------------------------------------------------------------"
for REPS in 3 10 30 50; do
  printf "REPS=%2d: " $REPS

  START=$(date +%s.%N)
  ./test/test_benchmark --benchmark_min_time=0.01s \
    --benchmark_repetitions=$REPS \
    >/dev/null 2>&1
  END=$(date +%s.%N)

  RUNTIME=$(echo "$END - $START" | bc)
  printf "Runtime=%6.2fs\n" $RUNTIME
done

echo ""
echo "Test 2: Varying min_time (REPS=10 constant)"
echo "----------------------------------------------------------------"
for MIN_TIME in 0.001 0.01 0.1 0.5; do
  printf "min_time=%5ss: " $MIN_TIME

  START=$(date +%s.%N)
  ./test/test_benchmark --benchmark_min_time=${MIN_TIME}s \
    --benchmark_repetitions=10 \
    >/dev/null 2>&1
  END=$(date +%s.%N)

  RUNTIME=$(echo "$END - $START" | bc)
  printf "Runtime=%6.2fs\n" $RUNTIME
done

echo ""
echo "================================================================"
echo "EXPECTED: Runtime should scale with REPS and with min_time"
