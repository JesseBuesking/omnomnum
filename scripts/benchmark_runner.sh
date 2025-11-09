#!/bin/bash
# Automated Benchmark Runner for OmNomNum
# This script ensures dependencies are installed and runs benchmarks with summary generation

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
RESULTS_DIR="$PROJECT_ROOT/benchmark_results"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
RESULT_FILE="$RESULTS_DIR/benchmark_${TIMESTAMP}.txt"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "=== OmNomNum Benchmark Runner ==="
echo

# Create results directory if it doesn't exist
mkdir -p "$RESULTS_DIR"

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to install re2c
install_re2c() {
    echo -e "${YELLOW}Installing re2c...${NC}"
    if command_exists apt-get; then
        sudo apt-get update && sudo apt-get install -y re2c
    elif command_exists yum; then
        sudo yum install -y re2c
    elif command_exists brew; then
        brew install re2c
    else
        echo -e "${YELLOW}Package manager not found, building re2c from source...${NC}"
        cd /tmp
        if [ -d "re2c" ]; then
            rm -rf re2c
        fi
        git clone --depth 1 https://github.com/skvadrik/re2c.git
        cd re2c
        mkdir build && cd build
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make -j4
        sudo cp re2c /usr/local/bin/
        cd "$PROJECT_ROOT"
    fi
    echo -e "${GREEN}re2c installed successfully${NC}"
}

# Function to install Google Benchmark
install_benchmark() {
    echo -e "${YELLOW}Installing Google Benchmark...${NC}"
    cd /tmp
    if [ -d "benchmark" ]; then
        rm -rf benchmark
    fi
    git clone --depth 1 https://github.com/google/benchmark.git
    cd benchmark
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true
    make -j4
    sudo make install
    cd "$PROJECT_ROOT"
    echo -e "${GREEN}Google Benchmark installed successfully${NC}"
}

# Check and install re2c if needed
echo "Checking for re2c..."
if command_exists re2c; then
    RE2C_VERSION=$(re2c --version 2>&1 | head -1)
    echo -e "${GREEN}✓ re2c found: $RE2C_VERSION${NC}"
else
    echo -e "${YELLOW}✗ re2c not found${NC}"
    install_re2c
fi
echo

# Check and install Google Benchmark if needed
echo "Checking for Google Benchmark..."
if [ -f "/usr/local/lib/libbenchmark.a" ] || [ -f "/lib/libbenchmark.a" ]; then
    echo -e "${GREEN}✓ Google Benchmark found${NC}"
else
    echo -e "${YELLOW}✗ Google Benchmark not found${NC}"
    install_benchmark
fi
echo

# Navigate to project root
cd "$PROJECT_ROOT"

# Clean and build
echo "Building project..."
make scanner.c
make clean
make benchmark

# Run benchmarks
echo
echo "Running benchmarks..."
echo "Results will be saved to: $RESULT_FILE"
echo

# Capture benchmark output
cd test
./test_benchmark --benchmark_min_time=2s --benchmark_repetitions=3 --benchmark_out=benchmark.json --benchmark_out_format=json | tee "$RESULT_FILE.raw"
cd ..

# Parse and summarize results
echo
echo "=== Benchmark Summary ===" | tee "$RESULT_FILE"
echo "Timestamp: $(date)" | tee -a "$RESULT_FILE"
echo "Git commit: $(git rev-parse --short HEAD)" | tee -a "$RESULT_FILE"
echo "Git status: $(git diff --quiet && echo 'clean' || echo 'dirty')" | tee -a "$RESULT_FILE"
echo | tee -a "$RESULT_FILE"

# Extract mean values from benchmark output
grep "_mean" "$RESULT_FILE.raw" | \
    awk '{printf "%-50s %10s ns\n", $1, $2}' | \
    sed 's|/repeats:.*_mean||' | \
    tee -a "$RESULT_FILE"

echo | tee -a "$RESULT_FILE"
echo "Full results saved to: $RESULT_FILE.raw" | tee -a "$RESULT_FILE"
echo -e "${GREEN}Benchmarking complete!${NC}"

# Optional: Compare with previous run if it exists
PREV_RESULT=$(ls -t "$RESULTS_DIR"/benchmark_*.txt 2>/dev/null | grep -v "$RESULT_FILE" | head -1)
if [ -n "$PREV_RESULT" ]; then
    echo
    echo "=== Comparison with previous run ===" | tee -a "$RESULT_FILE"
    echo "Previous: $(basename $PREV_RESULT)" | tee -a "$RESULT_FILE"
    echo | tee -a "$RESULT_FILE"

    # Simple comparison (just show both side by side)
    paste <(grep "BM_" "$PREV_RESULT" | awk '{print $1, $2}') \
          <(grep "BM_" "$RESULT_FILE" | awk '{print $2}') | \
    awk '{
        if (NF == 3) {
            prev = $2
            curr = $3
            pct_change = (curr - prev) / prev * 100
            printf "%-50s %10s -> %10s (%+6.1f%%)\n", $1, prev" ns", curr" ns", pct_change
        }
    }' | tee -a "$RESULT_FILE"
fi

echo
echo -e "${GREEN}Done! Results in: $RESULT_FILE${NC}"
