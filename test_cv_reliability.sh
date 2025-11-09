#!/bin/bash
# Test reliability of CV measurements themselves
# Question: If we run the same config 10 times, how much does the reported CV vary?

BINARY="./test/test_benchmark"

echo "Testing CV Measurement Reliability"
echo "Question: How stable are CV measurements across independent runs?"
echo "=" * 80

# Test different repetition counts
for REPS in 2 3 5 10; do
  echo ""
  echo "Testing with REPS=$REPS, min_time=0.005s (10 independent runs)"
  echo "Each run calculates CV from $REPS repetitions"
  echo "-" * 80

  REPORTED_CVS=()

  for run in {1..10}; do
    $BINARY --benchmark_min_time=0.005s --benchmark_repetitions=$REPS \
      --benchmark_filter="BM_simple" \
      --benchmark_out=test/cv_reliability_r${REPS}_run${run}.json \
      --benchmark_out_format=json >/dev/null 2>&1

    # Extract the CV that Google Benchmark reported
    CV=$(python3 << 'PYEOF'
import json, sys
with open(f"test/cv_reliability_r{sys.argv[1]}_run{sys.argv[2]}.json", 'r') as f:
    data = json.load(f)
cv_bench = next((b for b in data['benchmarks'] if 'BM_simple' in b['name'] and b.get('aggregate_name') == 'cv'), None)
if cv_bench:
    print(f"{cv_bench.get('real_time', 0):.3f}")
else:
    print("N/A")
PYEOF
 "$REPS" "$run")

    REPORTED_CVS+=($CV)
    printf "  Run %2d: Google Benchmark reported CV = %6s%%\n" $run "$CV"
  done

  # Now calculate CV of the CVs!
  echo ""
  python3 << 'PYEOF'
import sys
import math

cvs = [float(x) for x in sys.argv[2:] if x != 'N/A']
reps = sys.argv[1]

if len(cvs) >= 2:
    mean_cv = sum(cvs) / len(cvs)
    variance = sum((x - mean_cv) ** 2 for x in cvs) / (len(cvs) - 1)
    stddev = math.sqrt(variance)
    cv_of_cvs = (stddev / mean_cv * 100) if mean_cv > 0 else 0

    print(f"  Meta-Analysis for REPS={reps}:")
    print(f"    Mean of reported CVs:    {mean_cv:.3f}%")
    print(f"    Range of reported CVs:   {min(cvs):.3f}% - {max(cvs):.3f}%")
    print(f"    StdDev of reported CVs:  {stddev:.3f}%")
    print(f"    CV of the CVs:           {cv_of_cvs:.1f}%")

    if cv_of_cvs > 50:
        print(f"    ⚠️  UNRELIABLE: CV measurements vary by {cv_of_cvs:.0f}%!")
    elif cv_of_cvs > 25:
        print(f"    ⚠️  UNSTABLE: CV measurements have high variance")
    else:
        print(f"    ✓ STABLE: CV measurements are consistent")
else:
    print(f"  ERROR: Insufficient data for reps={reps}")
PYEOF "$REPS" "${REPORTED_CVS[@]}"
done

echo ""
echo "=" * 80
echo "INTERPRETATION:"
echo "  - If 'CV of the CVs' is high (>50%), the CV metric itself is unreliable"
echo "  - With REPS=2, variance calculation has only 1 degree of freedom"
echo "  - Need higher REPS for statistical reliability"
