#!/bin/bash
# Automated script to regenerate gperf perfect hash for denominator words
# This script:
# 1. Checks for gperf (installs if missing)
# 2. Generates perfect hash from denom_words.gperf
# 3. Transforms output with DENOM_ prefixes
# 4. Updates scanner.re with new hash table
# 5. Regenerates scanner.c with re2c
# 6. Optionally updates reference files

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

# Check for gperf, install if missing
check_gperf() {
    if ! command -v gperf &> /dev/null; then
        warn "gperf not found. Attempting to install..."

        # Detect package manager
        if command -v apt-get &> /dev/null; then
            info "Installing gperf via apt..."
            sudo apt-get update && sudo apt-get install -y gperf
        elif command -v yum &> /dev/null; then
            info "Installing gperf via yum..."
            sudo yum install -y gperf
        elif command -v dnf &> /dev/null; then
            info "Installing gperf via dnf..."
            sudo dnf install -y gperf
        elif command -v brew &> /dev/null; then
            info "Installing gperf via Homebrew..."
            brew install gperf
        else
            error "Could not detect package manager. Please install gperf manually."
        fi

        # Verify installation
        if ! command -v gperf &> /dev/null; then
            error "gperf installation failed"
        fi
    fi

    info "Using gperf: $(which gperf)"
    info "Version: $(gperf --version | head -1)"
}

# Check for re2c
check_re2c() {
    if ! command -v re2c &> /dev/null; then
        error "re2c not found. Please install re2c to regenerate scanner.c"
    fi
    info "Using re2c: $(which re2c)"
}

# Generate perfect hash with gperf (Preset B parameters)
generate_gperf() {
    local input_file="$1"
    local output_file="$2"

    info "Generating perfect hash from $input_file..."

    # Preset B flags: -C -l -c -t -m 100 -k '1,2,3,$'
    gperf -C -l -c -t -m 100 -k '1,2,3,$' "$input_file" > "$output_file"

    if [ $? -ne 0 ]; then
        error "gperf generation failed"
    fi

    info "Generated raw gperf output: $output_file"
}

# Transform gperf output to add DENOM_ prefixes and adjust function names
transform_gperf() {
    local raw_file="$1"
    local output_file="$2"

    info "Transforming gperf output with DENOM_ prefixes..."

    # Extract lines 35-268 (skip ANSI-C preamble and trailing line directive)
    # Add DENOM_ prefixes to constants and function names
    sed -n '35,268p' "$raw_file" | \
        sed 's/^#define TOTAL_KEYWORDS/#define DENOM_TOTAL_KEYWORDS/' | \
        sed 's/^#define MIN_WORD_LENGTH/#define DENOM_MIN_WORD_LENGTH/' | \
        sed 's/^#define MAX_WORD_LENGTH/#define DENOM_MAX_WORD_LENGTH/' | \
        sed 's/^#define MIN_HASH_VALUE/#define DENOM_MIN_HASH_VALUE/' | \
        sed 's/^#define MAX_HASH_VALUE/#define DENOM_MAX_HASH_VALUE/' | \
        sed 's/^hash (register/denom_hash (register/' | \
        sed 's/^in_word_set (register/denom_in_word_set (register/' | \
        sed 's/MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH/DENOM_MAX_WORD_LENGTH \&\& len >= DENOM_MIN_WORD_LENGTH/' | \
        sed 's/if (key <= MAX_HASH_VALUE)/if (key <= DENOM_MAX_HASH_VALUE)/' | \
        sed 's/ = hash (str, len)/ = denom_hash (str, len)/' \
        > "$output_file"

    if [ $? -ne 0 ]; then
        error "Transformation failed"
    fi

    info "Transformed output saved to: $output_file"
}

# Update scanner.re with new gperf section
update_scanner_re() {
    local gperf_section="$1"
    local scanner_file="scanner.re"

    info "Updating $scanner_file with new gperf section..."

    # Backup original
    cp "$scanner_file" "${scanner_file}.bak"

    # Extract lines 1-34 (before gperf section)
    sed -n '1,34p' "$scanner_file" > "${scanner_file}.new"

    # Add the new gperf section
    cat "$gperf_section" >> "${scanner_file}.new"

    # Add the END comment and rest of the file (from line 269 onwards)
    sed -n '269,$p' "$scanner_file" >> "${scanner_file}.new"

    # Replace original with new version
    mv "${scanner_file}.new" "$scanner_file"

    info "Updated $scanner_file successfully"
}

# Regenerate scanner.c from scanner.re
regenerate_scanner_c() {
    info "Regenerating scanner.c with re2c..."

    re2c -o scanner.c scanner.re

    if [ $? -ne 0 ]; then
        error "re2c regeneration failed"
    fi

    info "scanner.c regenerated successfully"
}

# Verify the build works
verify_build() {
    info "Verifying build..."

    make clean > /dev/null 2>&1 || true

    if make all > /tmp/build.log 2>&1; then
        info "Build succeeded!"
    else
        error "Build failed. Check /tmp/build.log for details"
    fi
}

# Run tests to verify correctness
run_tests() {
    info "Running test cases..."

    local failed=0

    # Test cases for halves
    test_case() {
        local input="$1"
        local expected="$2"
        local result=$(echo "$input" | ./omnomnum 2>/dev/null)

        if [ "$result" = "$expected" ]; then
            echo -e "  ${GREEN}✓${NC} '$input' → $result"
        else
            echo -e "  ${RED}✗${NC} '$input' → Expected: $expected, Got: $result"
            failed=$((failed + 1))
        fi
    }

    test_case "three halves" "3/2"
    test_case "two halves" "2/2"
    test_case "two and a half" "5/2"
    test_case "three and two halves" "8/2"
    test_case "one half" "1/2"

    if [ $failed -eq 0 ]; then
        info "All tests passed!"
        return 0
    else
        error "$failed test(s) failed"
    fi
}

# Update reference files
update_references() {
    info "Updating reference files..."

    if [ -f ".tmp/denom_preset_b_raw.c" ]; then
        cp ".tmp/denom_preset_b_raw.c" "denom_preset_b.c"
        info "Updated denom_preset_b.c"
    fi
}

# Main execution
main() {
    echo "========================================="
    echo "  gperf Perfect Hash Regeneration Tool"
    echo "========================================="
    echo ""

    # Create temp directory
    mkdir -p .tmp

    # Check dependencies
    check_gperf
    check_re2c
    echo ""

    # Verify input file exists
    if [ ! -f "denom_words.gperf" ]; then
        error "denom_words.gperf not found in current directory"
    fi

    # Generate and transform
    generate_gperf "denom_words.gperf" ".tmp/denom_preset_b_raw.c"
    transform_gperf ".tmp/denom_preset_b_raw.c" ".tmp/denom_section.c"
    echo ""

    # Update scanner files
    update_scanner_re ".tmp/denom_section.c"
    regenerate_scanner_c
    echo ""

    # Update reference files
    update_references
    echo ""

    # Verify
    verify_build
    echo ""

    # Run tests
    run_tests
    echo ""

    info "Regeneration complete!"
    echo ""
    echo "Modified files:"
    echo "  - scanner.re (updated gperf section)"
    echo "  - scanner.c (regenerated from scanner.re)"
    echo "  - denom_preset_b.c (reference file)"
    echo ""
    echo "Backup saved: scanner.re.bak"
    echo "Temporary files in: .tmp/"
    echo ""
}

# Run main function
main "$@"
