#!/usr/bin/env bash
set -euo pipefail

# Extract mean real_time for all benchmarks from Google Benchmark JSONs.
# Outputs CSV with all _mean benchmarks found in the input files.

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <JSON> [JSON ...]" >&2
  exit 1
fi

python3 - "$@" <<'PYTHON'
import json
import sys
import csv
from pathlib import Path

# Get all JSON files from arguments
json_files = sys.argv[1:]

if not json_files:
    print("No JSON files provided", file=sys.stderr)
    sys.exit(1)

# Extract all _mean benchmark names from first file to establish column order
with open(json_files[0]) as f:
    data = json.load(f)
    mean_benchmarks = [b['name'] for b in data['benchmarks'] if b['name'].endswith('_mean')]

# Simplify benchmark names for CSV headers (remove /real_time/threads:1 suffix)
def simplify_name(name):
    return name.replace('/real_time/threads:1_mean', '_ns').replace('_mean', '_ns')

headers = ['file'] + [simplify_name(name) for name in mean_benchmarks]

# Create CSV writer
writer = csv.writer(sys.stdout)
writer.writerow(headers)

# Extract values for each file
for json_file in json_files:
    with open(json_file) as f:
        data = json.load(f)
        bench_dict = {b['name']: b['real_time'] for b in data['benchmarks']}
        values = [json_file] + [bench_dict.get(name, '') for name in mean_benchmarks]
        writer.writerow(values)
PYTHON
