#!/usr/bin/env bash
set -euo pipefail

# Extract mean real_time for the three core benchmarks from Google Benchmark JSONs.
# Outputs CSV: file,BM_simple_ns,BM_long_string_ns,BM_many_numbers_ns

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <JSON> [JSON ...]" >&2
  exit 1
fi

echo "file,BM_simple_ns,BM_long_string_ns,BM_many_numbers_ns"
for f in "$@"; do
  awk -v file="$f" '
    /"name": "BM_simple_mean"/ { s=1; next }
    s && /"real_time":/ { s_val=$2; gsub(",","",s_val); s=0 }
    /"name": "BM_long_string_mean"/ { l=1; next }
    l && /"real_time":/ { l_val=$2; gsub(",","",l_val); l=0 }
    /"name": "BM_many_numbers_mean"/ { m=1; next }
    m && /"real_time":/ { m_val=$2; gsub(",","",m_val); m=0; printf "%s,%s,%s,%s\n", file, s_val, l_val, m_val }
  ' "$f" || true
done

