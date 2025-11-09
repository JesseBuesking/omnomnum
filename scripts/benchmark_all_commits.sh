#!/usr/bin/env bash
set -euo pipefail

# Script to benchmark all commits in the current branch
# Usage: bash scripts/benchmark_all_commits.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

RESULTS_DIR="$ROOT_DIR/benchmark_results"
mkdir -p "$RESULTS_DIR"

# Save current branch/commit
ORIGINAL_HEAD=$(git rev-parse HEAD)
ORIGINAL_BRANCH=$(git rev-parse --abbrev-ref HEAD)

echo "Starting benchmark across all commits..."
echo "Original: $ORIGINAL_BRANCH ($ORIGINAL_HEAD)"
echo "Results directory: $RESULTS_DIR"
echo ""

# Get all commit hashes (newest first)
COMMITS=($(git log --reverse --pretty=format:"%H"))
TOTAL=${#COMMITS[@]}

echo "Found $TOTAL commits to benchmark"
echo ""

# Function to build and run benchmark for current commit
run_benchmark() {
    local commit_hash=$1
    local commit_index=$2
    local short_hash=$(git rev-parse --short=8 "$commit_hash")
    local commit_msg=$(git log -1 --pretty=format:"%s" "$commit_hash")
    local result_file="$RESULTS_DIR/${commit_index}_${short_hash}.txt"

    echo "[$commit_index/$TOTAL] $short_hash: $commit_msg"

    # Clean build artifacts but preserve generated files tracked by git
    rm -f *.o grisu2/*.o branchlut/*.o bench_local bench_local_after bench_local_before test/test_benchmark test/test_benchmark.o 2>/dev/null || true

    # Try to build bench_local
    if make bench_local >/dev/null 2>&1; then
        # Run benchmark and save results
        echo "  Running benchmark..."
        {
            echo "Commit: $commit_hash"
            echo "Short: $short_hash"
            echo "Index: $commit_index"
            echo "Message: $commit_msg"
            echo "Date: $(git log -1 --pretty=format:'%ai' "$commit_hash")"
            echo "---"
            ./bench_local
        } > "$result_file" 2>&1
        echo "  ✓ Saved to $result_file"
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
        } > "$result_file"
    fi
    echo ""
}

# Loop through commits from oldest to newest
index=1
for commit in "${COMMITS[@]}"; do
    # Stash any changes before checkout
    git stash --quiet || true

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
git checkout "$ORIGINAL_BRANCH" >/dev/null 2>&1

echo ""
echo "Benchmark complete! Results in: $RESULTS_DIR"
echo "Total commits processed: $TOTAL"
