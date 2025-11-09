#!/bin/bash

BENCH_PREFIX=/home/user/omnomnum/vendor

echo "Rebuilding benchmark binary..."

rm -f test/test_benchmark.o test/test_benchmark

# Compile
c++ -std=c++17 -I"$BENCH_PREFIX"/include -I. \
  -DGIT_SHA=\"a21eafe\" \
  -DGIT_DESC=\"a21eafe\" \
  -DBUILD_TIME=\"2025-11-09T20:00:00Z\" \
  -DTREE_DIRTY_STR=\"1\" \
  -c test/test_benchmark.c -o test/test_benchmark.o

if [[ ! -f test/test_benchmark.o ]]; then
  echo "✗ Compilation failed"
  exit 1
fi

echo "✓ Compiled test_benchmark.o"

# Link
objs=$(ls -1 *.o grisu2/*.o branchlut/*.o 2>/dev/null | tr '\n' ' ')
c++ -std=c++17 -o test/test_benchmark -I. $objs -pthread -L"$BENCH_PREFIX"/lib -lbenchmark

if [[ ! -f test/test_benchmark ]]; then
  echo "✗ Linking failed"
  exit 1
fi

echo "✓ Linked test/test_benchmark"
echo ""
echo "Binary ready for testing!"
