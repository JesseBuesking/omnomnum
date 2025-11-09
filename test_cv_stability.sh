#!/bin/bash
# Test: Are CV measurements themselves stable across runs?

BINARY="./test/test_benchmark"

echo "CV MEASUREMENT STABILITY TEST"
echo "Testing if CV values are reproducible across independent runs"
echo "======================================================================"
echo ""

# Test different repetition counts
for REPS in 2 3 5 10; do
  echo "Testing REPS=$REPS, min_time=0.005s (10 independent runs)"
  echo "----------------------------------------------------------------------"

  for run in {1..10}; do
    $BINARY --benchmark_min_time=0.005s --benchmark_repetitions=$REPS \
      --benchmark_filter="BM_simple" \
      --benchmark_out=test/stability_r${REPS}_run${run}.json \
      --benchmark_out_format=json >/dev/null 2>&1
  done

  # Extract all CVs and analyze
  python3 << 'EOF'
import json
import sys
import math
import glob

reps = sys.argv[1]
files = sorted(glob.glob(f"test/stability_r{reps}_run*.json"))

cvs = []
for f in files:
    try:
        with open(f) as file:
            data = json.load(file)
        cv_bench = next((b for b in data['benchmarks']
                        if 'BM_simple' in b['name'] and b.get('aggregate_name') == 'cv'), None)
        if cv_bench:
            cvs.append(cv_bench.get('real_time', 0))
    except:
        pass

if len(cvs) >= 2:
    mean_cv = sum(cvs) / len(cvs)
    variance = sum((x - mean_cv) ** 2 for x in cvs) / (len(cvs) - 1)
    stddev = math.sqrt(variance)
    cv_of_cvs = (stddev / mean_cv * 100) if mean_cv > 0 else 0

    print(f"  Reported CVs: {', '.join(f'{cv:.3f}%' for cv in cvs)}")
    print(f"  Mean:         {mean_cv:.3f}%")
    print(f"  Range:        {min(cvs):.3f}% - {max(cvs):.3f}%")
    print(f"  StdDev:       {stddev:.3f}%")
    print(f"  CV of CVs:    {cv_of_cvs:.1f}%", end="")

    if cv_of_cvs > 50:
        print(" ⚠️  UNRELIABLE")
    elif cv_of_cvs > 25:
        print(" ⚠️  UNSTABLE")
    else:
        print(" ✓ STABLE")
else:
    print(f"  ERROR: Insufficient data")

print()
EOF
 "$REPS"
done

echo "======================================================================"
echo "INTERPRETATION:"
echo "  'CV of CVs' measures stability of CV measurements"
echo "  - High (>50%): CV metric unreliable with this REPS count"
echo "  - Medium (25-50%): CV metric somewhat unstable"
echo "  - Low (<25%): CV metric is reproducible"
