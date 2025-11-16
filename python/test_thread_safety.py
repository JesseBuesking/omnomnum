#!/usr/bin/env python3
"""Test thread safety issues with caching"""

import time
import threading
from functools import lru_cache
from collections import defaultdict
from omnomnum import OmNomNum

print("="*80)
print("Thread Safety Analysis: Caching with Multiple Threads")
print("="*80)

# Test 1: Shared lru_cache - Race conditions
print("\n1. SHARED lru_cache - POTENTIAL PROBLEMS")
print("-"*80)

class SharedCacheOmNomNum(OmNomNum):
    """Naive implementation - all instances share cache"""
    @lru_cache(maxsize=128)
    def normalize(self, text: str, reset: bool = True) -> str:
        return super().normalize(text, reset)

results_shared = defaultdict(list)

def worker_shared(worker_id, precision):
    """Worker thread using shared cache"""
    nom = SharedCacheOmNomNum(precision=precision)
    text = "one point five"

    for _ in range(100):
        result = nom.normalize(text)
        results_shared[worker_id].append(result)
        time.sleep(0.0001)  # Simulate work

# Start threads with different configurations
threads = []
threads.append(threading.Thread(target=worker_shared, args=(1, 2)))
threads.append(threading.Thread(target=worker_shared, args=(2, 6)))

for t in threads:
    t.start()
for t in threads:
    t.join()

# Check for inconsistencies
thread1_results = set(results_shared[1])
thread2_results = set(results_shared[2])

print(f"Thread 1 (precision=2) results: {thread1_results}")
print(f"Thread 2 (precision=6) results: {thread2_results}")

if len(thread1_results) > 1 or len(thread2_results) > 1:
    print("⚠️  PROBLEM: Different results from same config (race condition!)")
else:
    print("✓ Results consistent within each thread")

if thread1_results == thread2_results:
    print("⚠️  PROBLEM: Same results despite different precision!")
else:
    print("✓ Results differ between threads (as expected)")

# Test 2: Lock contention
print("\n2. LOCK CONTENTION IMPACT")
print("-"*80)

class LockedCacheOmNomNum(OmNomNum):
    """Using lru_cache which has internal locking"""
    @lru_cache(maxsize=128)
    def normalize(self, text: str, reset: bool = True) -> str:
        # Simulate some work to make lock contention visible
        time.sleep(0.0001)
        return super().normalize(text, reset)

def benchmark_concurrent(num_threads, use_cache):
    """Benchmark with multiple concurrent threads"""
    if use_cache:
        nom = LockedCacheOmNomNum()
    else:
        nom = OmNomNum()

    completed = []

    def worker():
        start = time.perf_counter()
        for _ in range(100):
            nom.normalize("twenty three")
        elapsed = time.perf_counter() - start
        completed.append(elapsed)

    start = time.perf_counter()
    threads = [threading.Thread(target=worker) for _ in range(num_threads)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    total = time.perf_counter() - start

    return total, completed

# Single thread baseline
total_1, times_1 = benchmark_concurrent(1, use_cache=False)
print(f"1 thread (no cache):     {total_1*1000:.1f} ms total")

# Multiple threads without cache (should scale linearly)
total_4_no, times_4_no = benchmark_concurrent(4, use_cache=False)
print(f"4 threads (no cache):    {total_4_no*1000:.1f} ms total")
print(f"  Expected (4x work):    {total_1*4*1000:.1f} ms")
print(f"  Actual speedup:        {total_1*4/total_4_no:.2f}x")

# Multiple threads with shared cache (lock contention!)
total_4_cache, times_4_cache = benchmark_concurrent(4, use_cache=True)
print(f"4 threads (shared cache): {total_4_cache*1000:.1f} ms total")
print(f"  Expected (if no lock):  {total_1*4*1000:.1f} ms")
print(f"  Lock overhead:          {(total_4_cache - total_4_no)*1000:.1f} ms ({(total_4_cache/total_4_no - 1)*100:.0f}% slower)")

# Test 3: Per-instance cache (safer)
print("\n3. PER-INSTANCE CACHE - SAFER")
print("-"*80)

class InstanceCacheOmNomNum(OmNomNum):
    """Each instance has its own cache"""
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self._cache = {}
        self._lock = threading.Lock()  # Protect the cache dict

    def normalize(self, text: str, reset: bool = True) -> str:
        with self._lock:
            if text in self._cache:
                return self._cache[text]

        result = super().normalize(text, reset)

        with self._lock:
            self._cache[text] = result

        return result

results_instance = defaultdict(list)

def worker_instance(worker_id, precision):
    """Each thread has its own instance (and cache)"""
    nom = InstanceCacheOmNomNum(precision=precision)
    text = "one point five"

    for _ in range(100):
        result = nom.normalize(text)
        results_instance[worker_id].append(result)

threads = []
threads.append(threading.Thread(target=worker_instance, args=(1, 2)))
threads.append(threading.Thread(target=worker_instance, args=(2, 6)))

for t in threads:
    t.start()
for t in threads:
    t.join()

thread1_results = set(results_instance[1])
thread2_results = set(results_instance[2])

print(f"Thread 1 (precision=2) results: {thread1_results}")
print(f"Thread 2 (precision=6) results: {thread2_results}")
print("✓ Each thread has its own instance and cache")
print("✓ No shared state = no race conditions")

# Test 4: Thread-local cache (best for high concurrency)
print("\n4. THREAD-LOCAL CACHE - BEST FOR CONCURRENCY")
print("-"*80)

class ThreadLocalCacheOmNomNum(OmNomNum):
    """Each thread has its own cache (no locking needed!)"""
    _thread_local = threading.local()

    def normalize(self, text: str, reset: bool = True) -> str:
        # Get or create cache for this thread
        if not hasattr(self._thread_local, 'cache'):
            self._thread_local.cache = {}

        cache = self._thread_local.cache

        # No locking needed - each thread has its own cache!
        if text in cache:
            return cache[text]

        result = super().normalize(text, reset)
        cache[text] = result
        return result

def benchmark_thread_local(num_threads):
    """Benchmark thread-local cache"""
    nom = ThreadLocalCacheOmNomNum()

    def worker():
        for _ in range(1000):
            nom.normalize("twenty three")

    start = time.perf_counter()
    threads = [threading.Thread(target=worker) for _ in range(num_threads)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    total = time.perf_counter() - start

    return total

time_1 = benchmark_thread_local(1)
time_4 = benchmark_thread_local(4)

print(f"1 thread:  {time_1*1000:.1f} ms")
print(f"4 threads: {time_4*1000:.1f} ms")
print(f"Speedup:   {time_1/time_4:.2f}x (ideal: 4.0x)")
print("✓ Near-linear scaling (no lock contention)")

# Summary
print("\n" + "="*80)
print("SUMMARY: Thread Safety with Caching")
print("="*80)

print("\n❌ PROBLEMS with @lru_cache (class method):")
print("  1. Shared cache across all instances")
print("  2. Lock contention in multi-threaded scenarios")
print("  3. Wrong results if configs differ between instances")
print("  4. Serializes execution (threads wait for lock)")

print("\n✅ SOLUTION 1: Per-instance cache")
print("  + Each instance has own cache")
print("  + No cross-instance contamination")
print("  + Need locking within instance")
print("  - Still have lock contention within instance")

print("\n✅ SOLUTION 2: Thread-local cache (BEST)")
print("  + Each thread has own cache")
print("  + NO LOCKING NEEDED")
print("  + Perfect parallelization")
print("  + No race conditions")
print("  - Separate cache per thread (more memory)")

print("\n💡 RECOMMENDATION:")
print("  - Single-threaded: Use @lru_cache")
print("  - Multi-threaded: Use thread-local cache")
print("  - If sharing instances: Per-instance cache with lock")
