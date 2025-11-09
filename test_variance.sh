#!/bin/bash
# Test different min_time and repetition combinations to find minimum with acceptable variance

BINARY="./test/test_benchmark"

if [[ ! -f "$BINARY" ]]; then
  echo "Error: benchmark binary not found. Please build first."
  exit 1
fi

echo "Testing variance with different configurations..."
echo "Target: CV < 3.6% for acceptable variance"
echo ""

# Function to extract max CV from benchmark results
extract_max_cv() {
  local json_file=$1
  if [[ ! -f "$json_file" ]]; then
    echo "N/A"
    return
  fi
  # Extract all CV values and find the maximum
  grep '"cv"' "$json_file" | grep -oP ':\s*\K[0-9.]+' | sort -n -r | head -1
}

# Test configuration 1: reps=2, min_time=0.5s
echo "[1/6] Testing: reps=2, min_time=0.5s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.5s --benchmark_repetitions=2 \
  --benchmark_out=test/var_r2_t0.5.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r2_t0.5.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

# Test configuration 2: reps=2, min_time=0.25s
echo "[2/6] Testing: reps=2, min_time=0.25s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.25s --benchmark_repetitions=2 \
  --benchmark_out=test/var_r2_t0.25.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r2_t0.25.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

# Test configuration 3: reps=2, min_time=0.1s
echo "[3/6] Testing: reps=2, min_time=0.1s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.1s --benchmark_repetitions=2 \
  --benchmark_out=test/var_r2_t0.1.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r2_t0.1.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

# Test configuration 4: reps=3, min_time=0.25s
echo "[4/6] Testing: reps=3, min_time=0.25s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.25s --benchmark_repetitions=3 \
  --benchmark_out=test/var_r3_t0.25.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r3_t0.25.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

# Test configuration 5: reps=3, min_time=0.1s
echo "[5/6] Testing: reps=3, min_time=0.1s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.1s --benchmark_repetitions=3 \
  --benchmark_out=test/var_r3_t0.1.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r3_t0.1.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

# Test configuration 6: reps=2, min_time=0.05s (very aggressive)
echo "[6/6] Testing: reps=2, min_time=0.05s"
START=$(date +%s)
$BINARY --benchmark_min_time=0.05s --benchmark_repetitions=2 \
  --benchmark_out=test/var_r2_t0.05.json --benchmark_out_format=json >/dev/null 2>&1
END=$(date +%s)
MAX_CV=$(extract_max_cv test/var_r2_t0.05.json)
echo "  Result: Max CV = ${MAX_CV}%, Time = $((END - START))s"

echo ""
echo "Summary (Target: CV < 3.6%):"
echo "=========================================="
echo "Config                 | Max CV  | Time  | Status"
echo "----------------------|---------|-------|--------"
printf "reps=2, min_time=0.5s  | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r2_t0.5.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r2_t0.5.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
printf "reps=2, min_time=0.25s | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r2_t0.25.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r2_t0.25.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
printf "reps=2, min_time=0.1s  | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r2_t0.1.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r2_t0.1.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
printf "reps=3, min_time=0.25s | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r3_t0.25.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r3_t0.25.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
printf "reps=3, min_time=0.1s  | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r3_t0.1.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r3_t0.1.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
printf "reps=2, min_time=0.05s | %6.2f%% | %4ds  | " $(extract_max_cv test/var_r2_t0.05.json | bc) 0
[[ $(echo "$(extract_max_cv test/var_r2_t0.05.json) < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL"
