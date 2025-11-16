#!/usr/bin/env bash
set -euo pipefail

echo "=== Cleanup Script for Temporary Benchmark/Analysis Files ==="
echo ""
echo "This will DELETE the following files:"
echo ""

# Analysis/report files
echo "Analysis & Reports:"
echo "  - final_analysis.txt"
echo "  - analysis_20.txt"
echo "  - analysis_40.txt"
echo "  - analysis_60.txt"
echo "  - analysis_80.txt"
echo "  - analysis_100.txt"
echo "  - analysis_120.txt"
echo "  - benchmark_progress_report.txt"
echo "  - commit_diffs_summary.txt"
echo ""

# Log files
echo "Log Files:"
echo "  - benchmark_sweep.log"
echo "  - benchmark_sweep_full.log"
echo "  - test_all_builds.log"
echo "  - test_all_builds_fixed.log"
echo "  - test_all_builds_new.log"
echo "  - build_test_results.txt"
echo "  - test_135_results.txt"
echo ""

# Profiling files
echo "Profiling Files:"
echo "  - profile_full.txt"
echo "  - profile_many.txt"
echo "  - profile_optimized.txt"
echo "  - profile_simple.txt"
echo "  - profile_test (binary)"
echo "  - profile_test.c"
echo ""

# Scripts
echo "Temporary Scripts:"
echo "  - analyze_partial_results.py"
echo "  - continuous_monitor.sh"
echo "  - monitor_progress.sh"
echo "  - wait_and_analyze.sh"
echo "  - debug_failed_commit.sh"
echo "  - debug_one_commit.sh"
echo "  - test_5_builds_only.sh"
echo "  - test_5_commits.sh"
echo "  - test_all_135_builds.sh"
echo "  - test_all_5.sh"
echo "  - test_all_commits_build.sh"
echo "  - test_builds.sh"
echo "  - test_commit_benchmarks.sh"
echo "  - test_failed_builds.sh"
echo "  - test_sweep_fix.sh"
echo "  - analyze_key_commits.sh"
echo "  - get_commit_details.sh"
echo "  - get_commit_diffs.sh"
echo "  - run_full_sweep_when_ready.sh"
echo ""

# Test benchmark JSONs
echo "Test Benchmark JSON Files (in test/):"
echo "  - baseline_HEAD.json"
echo "  - baseline_extended.json"
echo "  - baseline_percent.json"
echo "  - opt_buffer_reuse.json"
echo "  - opt_buffer_reuse2.json"
echo "  - opt_fractions.json"
echo "  - opt_fractions2.json"
echo "  - opt_fractions_extended.json"
echo "  - opt_fractions_long.json"
echo "  - opt_percent.json"
echo "  - opt_prereserve.json"
echo "  - opt_prereserve2.json"
echo ""

echo "KEEPING (useful files):"
echo "  ✓ BENCHMARK_HISTORY_SUMMARY.md"
echo "  ✓ analyze_benchmark_results.py"
echo "  ✓ benchmark_history/ directory (with all JSON data)"
echo ""

read -p "Proceed with deletion? (y/N): " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Cancelled."
    exit 0
fi

echo ""
echo "Deleting files..."

# Analysis/reports
rm -f final_analysis.txt
rm -f analysis_20.txt analysis_40.txt analysis_60.txt analysis_80.txt analysis_100.txt analysis_120.txt
rm -f benchmark_progress_report.txt
rm -f commit_diffs_summary.txt

# Logs
rm -f benchmark_sweep.log benchmark_sweep_full.log
rm -f test_all_builds.log test_all_builds_fixed.log test_all_builds_new.log
rm -f build_test_results.txt test_135_results.txt

# Profiling
rm -f profile_full.txt profile_many.txt profile_optimized.txt profile_simple.txt
rm -f profile_test profile_test.c

# Scripts
rm -f analyze_partial_results.py
rm -f continuous_monitor.sh monitor_progress.sh wait_and_analyze.sh
rm -f debug_failed_commit.sh debug_one_commit.sh
rm -f test_5_builds_only.sh test_5_commits.sh
rm -f test_all_135_builds.sh test_all_5.sh test_all_commits_build.sh
rm -f test_builds.sh test_commit_benchmarks.sh
rm -f test_failed_builds.sh test_sweep_fix.sh
rm -f analyze_key_commits.sh get_commit_details.sh get_commit_diffs.sh
rm -f run_full_sweep_when_ready.sh

# Test JSONs
rm -f test/baseline_HEAD.json test/baseline_extended.json test/baseline_percent.json
rm -f test/opt_buffer_reuse.json test/opt_buffer_reuse2.json
rm -f test/opt_fractions.json test/opt_fractions2.json
rm -f test/opt_fractions_extended.json test/opt_fractions_long.json
rm -f test/opt_percent.json test/opt_prereserve.json test/opt_prereserve2.json

echo ""
echo "✓ Cleanup complete!"
echo ""
echo "Remaining untracked files:"
git status --short --untracked-files=all | grep "^??" || echo "(none)"
