#!/bin/bash
# Find exact CV failure threshold between 0.01s and 0.005s

BINARY="./test/test_benchmark"
REPS=2

echo "Finding exact CV failure threshold (target: CV < 3.6%)"
echo "Testing with repetitions=$REPS"
echo "="

# Function to run benchmark and extract max CV
run_and_extract_cv() {
  local min_time=$1
  local output_file=$2

  $BINARY --benchmark_min_time=${min_time}s --benchmark_repetitions=$REPS \
    --benchmark_out=$output_file --benchmark_out_format=json >/dev/null 2>&1

  python3 << EOF
import json
with open("$output_file", 'r') as f:
    data = json.load(f)
max_cv = max([b.get('real_time', 0) for b in data['benchmarks'] if b.get('aggregate_name') == 'cv'], default=0)
print(f"{max_cv:.2f}")
EOF
}

# Test 0.01s (baseline - should pass) - 5 runs
echo "[1/4] Testing min_time=0.01s (5 runs)"
RESULTS_01=()
for i in {1..5}; do
  CV=$(run_and_extract_cv 0.01 "test/threshold_0.01_run${i}.json")
  RESULTS_01+=($CV)
  STATUS=$([[ $(echo "$CV < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL")
  echo "  Run $i: CV=${CV}% $STATUS"
done

# Test 0.0075s (3/4 point) - 5 runs
echo ""
echo "[2/4] Testing min_time=0.0075s (5 runs)"
RESULTS_0075=()
for i in {1..5}; do
  CV=$(run_and_extract_cv 0.0075 "test/threshold_0.0075_run${i}.json")
  RESULTS_0075+=($CV)
  STATUS=$([[ $(echo "$CV < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL")
  echo "  Run $i: CV=${CV}% $STATUS"
done

# Test 0.006s - 5 runs
echo ""
echo "[3/4] Testing min_time=0.006s (5 runs)"
RESULTS_006=()
for i in {1..5}; do
  CV=$(run_and_extract_cv 0.006 "test/threshold_0.006_run${i}.json")
  RESULTS_006+=($CV)
  STATUS=$([[ $(echo "$CV < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL")
  echo "  Run $i: CV=${CV}% $STATUS"
done

# Test 0.005s (should fail) - 5 runs
echo ""
echo "[4/4] Testing min_time=0.005s (5 runs)"
RESULTS_005=()
for i in {1..5}; do
  CV=$(run_and_extract_cv 0.005 "test/threshold_0.005_run${i}.json")
  RESULTS_005+=($CV)
  STATUS=$([[ $(echo "$CV < 3.6" | bc -l) == 1 ]] && echo "✓ PASS" || echo "✗ FAIL")
  echo "  Run $i: CV=${CV}% $STATUS"
done

# Summary
echo ""
echo "="
echo "SUMMARY - Threshold Analysis"
echo "="
echo ""
echo "min_time=0.01s:   ${RESULTS_01[@]}"
echo "min_time=0.0075s: ${RESULTS_0075[@]}"
echo "min_time=0.006s:  ${RESULTS_006[@]}"
echo "min_time=0.005s:  ${RESULTS_005[@]}"
echo ""

# Calculate averages using Python for reliability
python3 << 'PYEOF'
import sys

def analyze(name, values):
    vals = [float(v) for v in values]
    avg = sum(vals) / len(vals)
    max_val = max(vals)
    min_val = min(vals)
    pass_count = sum(1 for v in vals if v < 3.6)
    fail_count = len(vals) - pass_count

    status = "RELIABLE PASS" if pass_count == 5 else "RELIABLE FAIL" if fail_count == 5 else "INCONSISTENT"

    print(f"{name:20} | Avg: {avg:5.2f}% | Range: {min_val:5.2f}%-{max_val:5.2f}% | {pass_count}/5 pass | {status}")
    return avg < 3.6

print("Config               | Average CV | Range         | Pass Rate | Status")
print("-" * 85)
results = {}
results['0.01s'] = analyze("min_time=0.01s", sys.argv[1:6])
results['0.0075s'] = analyze("min_time=0.0075s", sys.argv[6:11])
results['0.006s'] = analyze("min_time=0.006s", sys.argv[11:16])
results['0.005s'] = analyze("min_time=0.005s", sys.argv[16:21])

print("\n" + "=" * 85)
print("CONCLUSION:")
# Find the threshold
if results['0.0075s']:
    if results['0.006s']:
        print("✓ Failure threshold is between 0.006s and 0.005s")
        print("✓ RECOMMENDED MINIMUM: min_time=0.006s (reps=2)")
    else:
        print("✓ Failure threshold is between 0.0075s and 0.006s")
        print("✓ RECOMMENDED MINIMUM: min_time=0.0075s (reps=2)")
elif results['0.01s']:
    print("✓ Failure threshold is between 0.01s and 0.0075s")
    print("✓ RECOMMENDED MINIMUM: min_time=0.01s (reps=2)")
else:
    print("✗ Unexpected results - 0.01s should be reliable")
PYEOF "${RESULTS_01[@]}" "${RESULTS_0075[@]}" "${RESULTS_006[@]}" "${RESULTS_005[@]}"
