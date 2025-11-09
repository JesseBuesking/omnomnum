#!/usr/bin/env bash
set -euo pipefail

RESULTS_DIR="optimization_benchmarks"
mkdir -p "$RESULTS_DIR"

echo "================================================================================"
echo "BENCHMARKING EACH OPTIMIZATION WITH GOOGLE BENCHMARK"
echo "================================================================================"
echo

# Function to build and benchmark
bench_step() {
    local name="$1"
    local desc="$2"

    echo "[$name] $desc"
    echo "Building..."

    make clean >/dev/null 2>&1
    make all >/dev/null 2>&1
    make test/test_benchmark BENCH_PREFIX=/usr/local >/dev/null 2>&1

    echo "Running Google Benchmark..."
    ./test/test_benchmark \
        --benchmark_min_time=1s \
        --benchmark_repetitions=5 \
        --benchmark_filter="BM_many_numbers/repeats" \
        --benchmark_out="$RESULTS_DIR/${name}.json" \
        --benchmark_out_format=json 2>&1 | grep "mean"

    echo
}

# BASELINE
echo "================================================================================"
git restore scanner.def.c omnomnum.c 2>/dev/null || true
bench_step "00_baseline" "Baseline (no optimizations)"

# OPT 1: Buffer reuse
echo "================================================================================"
git restore scanner.def.c omnomnum.c 2>/dev/null || true

# Apply Opt 1
sed -i '98s/state->result = NULL;/state->result = sdsempty();  \/\/ OPT1/' scanner.def.c
sed -i '108s/if (state->result) { sdsfree(state->result); state->result = NULL; }/if (state->result) sdsclear(state->result);  \/\/ OPT1/' scanner.def.c
sed -i '122a\    if (state->result) { sdsfree(state->result); state->result = NULL; }  \/\/ OPT1' scanner.def.c
sed -i '432s/state->result = sdsempty();/sdsclear(state->result);  \/\/ OPT1/' omnomnum.c
sed -i '534s/state->result = sdsempty();/sdsclear(state->result);  \/\/ OPT1/' omnomnum.c

bench_step "01_buffer_reuse" "OPT1: SDS Buffer Reuse"

# OPT 2: YYSTYPE capacity
echo "================================================================================"
git restore scanner.def.c omnomnum.c 2>/dev/null || true

# Apply Opt 2
sed -i '103s/initYYSTYPEList(&(state->yystypeList), 4);/initYYSTYPEList(\&(state->yystypeList), 128);  \/\/ OPT2/' scanner.def.c

bench_step "02_yystype_capacity" "OPT2: Larger YYSTYPE Capacity (4→128)"

# OPT 1+2: Combined
echo "================================================================================"
git restore scanner.def.c omnomnum.c 2>/dev/null || true

# Apply both
sed -i '98s/state->result = NULL;/state->result = sdsempty();/' scanner.def.c
sed -i '108s/if (state->result) { sdsfree(state->result); state->result = NULL; }/if (state->result) sdsclear(state->result);/' scanner.def.c
sed -i '122a\    if (state->result) { sdsfree(state->result); state->result = NULL; }' scanner.def.c
sed -i '432s/state->result = sdsempty();/sdsclear(state->result);/' omnomnum.c
sed -i '534s/state->result = sdsempty();/sdsclear(state->result);/' omnomnum.c
sed -i '103s/initYYSTYPEList(&(state->yystypeList), 4);/initYYSTYPEList(\&(state->yystypeList), 128);/' scanner.def.c

bench_step "03_combined" "OPT1+2: Combined"

echo "================================================================================"
echo "PARSING RESULTS"
echo "================================================================================"

python3 << 'PYEOF'
import json
import glob

results = {}
for f in sorted(glob.glob("optimization_benchmarks/*.json")):
    name = f.split("/")[-1].replace(".json", "")
    with open(f) as fp:
        data = json.load(fp)
        for bm in data["benchmarks"]:
            if bm.get("aggregate_name") == "mean":
                results[name] = {
                    "real_time": bm["real_time"],
                    "cpu_time": bm["cpu_time"],
                    "stddev": next((b["real_time"] for b in data["benchmarks"] if b.get("aggregate_name") == "stddev"), 0)
                }

print("\n" + "="*80)
print("OPTIMIZATION RESULTS (nanoseconds)")
print("="*80)
print(f"{'Step':<25} {'Real Time (ns)':<18} {'vs Baseline':<15} {'Improvement'}")
print("-"*80)

baseline = results["00_baseline"]["real_time"]
for step in sorted(results.keys()):
    rt = results[step]["real_time"]
    std = results[step]["stddev"]
    diff = baseline - rt
    pct = (diff / baseline) * 100

    step_name = step.replace("_", " ").title()
    print(f"{step_name:<25} {rt:>10.2f} ± {std:>6.2f}   {diff:>+8.2f} ns   {pct:>+6.2f}%")

print("="*80)
PYEOF

echo
echo "Results saved in: $RESULTS_DIR/"
