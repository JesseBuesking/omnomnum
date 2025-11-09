#!/usr/bin/env bash
set -euo pipefail

# Script to benchmark all commits using Google Benchmark
# Usage: bash scripts/benchmark_all_commits_gb.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

RESULTS_DIR="$ROOT_DIR/benchmark_results_gb"
mkdir -p "$RESULTS_DIR"

# Save current branch/commit
ORIGINAL_HEAD=$(git rev-parse HEAD)
ORIGINAL_BRANCH=$(git rev-parse --abbrev-ref HEAD)

echo "Starting Google Benchmark across all commits..."
echo "Original: $ORIGINAL_BRANCH ($ORIGINAL_HEAD)"
echo "Results directory: $RESULTS_DIR"
echo ""

# Get all commit hashes (oldest first for reverse chronological)
COMMITS=($(git log --reverse --pretty=format:"%H"))
TOTAL=${#COMMITS[@]}

echo "Found $TOTAL commits to benchmark"
echo ""

# Function to build and run Google Benchmark for current commit
run_benchmark() {
    local commit_hash=$1
    local commit_index=$2
    local short_hash=$(git rev-parse --short=8 "$commit_hash")
    local commit_msg=$(git log -1 --pretty=format:"%s" "$commit_hash")
    local result_json="$RESULTS_DIR/${commit_index}_${short_hash}.json"
    local result_txt="$RESULTS_DIR/${commit_index}_${short_hash}.txt"

    echo "[$commit_index/$TOTAL] $short_hash: $commit_msg"

    # Clean build artifacts but preserve generated files tracked by git
    rm -f *.o grisu2/*.o branchlut/*.o test/test_benchmark test/test_benchmark.o 2>/dev/null || true

    # Try to build test_benchmark
    if make test/test_benchmark BENCH_PREFIX=/usr/local >/dev/null 2>&1; then
        # Run Google Benchmark and save results
        echo "  Running Google Benchmark..."
        {
            echo "Commit: $commit_hash"
            echo "Short: $short_hash"
            echo "Index: $commit_index"
            echo "Message: $commit_msg"
            echo "Date: $(git log -1 --pretty=format:'%ai' "$commit_hash")"
            echo "---"
            ./test/test_benchmark \
                --benchmark_min_time=1s \
                --benchmark_repetitions=3 \
                --benchmark_out="$result_json" \
                --benchmark_out_format=json 2>&1
        } > "$result_txt" 2>&1
        echo "  ✓ Saved to $result_json"
    else
        echo "  ✗ Build failed, skipping"
        {
            echo "Commit: $commit_hash"
            echo "Short: $short_hash"
            echo "Index: $commit_index"
            echo "Message: $commit_msg"
            echo "Date: $(git log -1 --pretty=format:'%ai' "$commit_hash")"
            echo "---"
            echo "BUILD FAILED"
        } > "$result_txt"
    fi
    echo ""
}

# Loop through commits from oldest to newest
index=1
for commit in "${COMMITS[@]}"; do
    # Stash any changes before checkout
    git stash --quiet 2>/dev/null || true

    git checkout "$commit" --quiet 2>/dev/null || {
        echo "  ✗ Failed to checkout $commit, skipping"
        index=$((index + 1))
        continue
    }
    run_benchmark "$commit" "$index"
    index=$((index + 1))
done

# Return to original state
echo "Returning to $ORIGINAL_BRANCH..."
git checkout "$ORIGINAL_BRANCH" --quiet 2>&1

echo ""
echo "Google Benchmark complete! Results in: $RESULTS_DIR"
echo "Total commits processed: $TOTAL"
