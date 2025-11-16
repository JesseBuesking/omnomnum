# CFFI Alternatives for Python-C Bindings

## Current Performance: CFFI

**OmNomNum with CFFI:**
- Total time: 0.687 µs
- CFFI overhead: 0.320 µs (47% of total!)
  - encode/decode: 0.134 µs
  - Result extraction: 0.149 µs
  - State reset calls: 0.085 µs

**CFFI overhead breakdown:**
```
Python str → C char*:        0.134 µs (20%)
C sds → Python str:          0.149 µs (22%)
Function call overhead:      0.037 µs (5%)
Total CFFI tax:              0.320 µs (47%)
```

## Alternative 1: Python C API (Native) 🟢

**What it is**: Python's native C extension API

**Performance**: ⭐⭐⭐⭐⭐ (Fastest possible)
- **Zero** abstraction overhead
- Direct PyObject manipulation
- **Estimated overhead: 0.05-0.10 µs (~7-15%)**
- **Speedup vs CFFI: 3-6x faster for boundary crossing**

**Pros**:
- Absolute fastest performance
- No dependencies
- Full control
- What CPython itself uses

**Cons**:
- Very verbose boilerplate
- Manual reference counting (memory leaks risk)
- Error-prone (segfaults)
- Python version-specific
- Huge effort to rewrite (~1000+ lines)

**Example complexity**:
```c
static PyObject* py_normalize(PyObject* self, PyObject* args) {
    const char* data;
    Py_ssize_t data_len;
    ParserState* state;

    // Parse arguments (verbose!)
    if (!PyArg_ParseTuple(args, "s#O", &data, &data_len, &state_obj)) {
        return NULL;
    }

    // Extract C pointer from capsule (more code)
    state = (ParserState*)PyCapsule_GetPointer(state_obj, "ParserState");
    if (!state) return NULL;

    // Call C function
    normalize(data, data_len, state);

    // Convert result back to Python (more code)
    if (!state->result) {
        Py_RETURN_NONE;
    }

    PyObject* result = PyUnicode_FromStringAndSize(state->result, sdslen(state->result));
    if (!result) {
        return NULL;
    }

    return result;
}

// Module initialization (100+ more lines)
// Method definitions
// Type definitions
// etc...
```

**Estimated effort**: 1-2 weeks
**Risk**: High (memory leaks, crashes)
**Gain**: 0.22-0.27 µs (32-40% faster)
**Worth it**: Only if you need absolute maximum performance

---

## Alternative 2: Cython 🟡

**What it is**: Python-like language that compiles to C extensions

**Performance**: ⭐⭐⭐⭐ (Very fast)
- Minimal overhead for well-written code
- **Estimated overhead: 0.08-0.12 µs (~12-18%)**
- **Speedup vs CFFI: 2-4x faster for boundary crossing**

**Pros**:
- Much easier than raw C API
- Python-like syntax
- Can gradually optimize hot paths
- Good documentation
- Automatic reference counting

**Cons**:
- New build dependency (Cython compiler)
- Another language to learn
- Still requires understanding C types
- Effort to rewrite: ~2-3 days

**Example code**:
```python
# cython: language_level=3

from libc.stdlib cimport malloc, free

cdef extern from "omnomnum.h":
    ctypedef struct ParserState:
        char* result
        int precision

    void normalize(const char* data, size_t len, ParserState* state)
    void initParserState(ParserState* state)

def py_normalize(str text):
    cdef ParserState state
    cdef bytes text_bytes = text.encode('utf-8')
    cdef const char* c_text = text_bytes

    initParserState(&state)
    normalize(c_text, len(text_bytes), &state)

    if state.result:
        return state.result.decode('utf-8')
    return ""
```

**Estimated effort**: 2-3 days
**Risk**: Medium
**Gain**: 0.18-0.24 µs (26-35% faster)
**Worth it**: Good balance of performance and effort

---

## Alternative 3: PyO3 (Rust) 🟡

**What it is**: Rust bindings for Python (what text2num uses!)

**Performance**: ⭐⭐⭐⭐⭐ (Fastest, tied with C API)
- Highly optimized by Rust team
- **Estimated overhead: 0.05-0.08 µs (~7-12%)**
- **Speedup vs CFFI: 4-6x faster for boundary crossing**

**Pros**:
- Excellent performance (close to native C API)
- Memory safety (no segfaults!)
- Modern tooling
- Easy to use compared to C API
- This is why text2num is fast!

**Cons**:
- **Requires rewriting in Rust** (not just bindings!)
- Can't use existing C code directly
- Completely different language
- Effort to rewrite: **2-3 months**

**Example code**:
```rust
use pyo3::prelude::*;

#[pyclass]
struct OmNomNum {
    precision: usize,
}

#[pymethods]
impl OmNomNum {
    #[new]
    fn new(precision: usize) -> Self {
        OmNomNum { precision }
    }

    fn normalize(&self, text: &str) -> PyResult<String> {
        // Would need to rewrite entire normalize() in Rust
        Ok(text.to_string())
    }
}

#[pymodule]
fn omnomnum(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add_class::<OmNomNum>()?;
    Ok(())
}
```

**Estimated effort**: 2-3 months (full rewrite)
**Risk**: High (complete rewrite)
**Gain**: 0.22-0.27 µs (32-40% faster) + Rust performance for C code
**Worth it**: **Only if you want Rust's speed AND memory safety**. But then why not just use text2num?

---

## Alternative 4: pybind11 (C++) 🟡

**What it is**: C++11 library for Python bindings

**Performance**: ⭐⭐⭐⭐ (Very fast)
- Efficient for C++ code
- **Estimated overhead: 0.08-0.15 µs (~12-22%)**
- **Speedup vs CFFI: 2-4x faster**

**Pros**:
- Cleaner than raw C API
- Good for C++ projects
- Header-only library
- Modern C++ features

**Cons**:
- OmNomNum is C, not C++ (would need wrapper)
- Another dependency
- Template compilation can be slow
- Effort: 1 week

**Not ideal for C code like OmNomNum**

---

## Alternative 5: nanobind (C++) 🟡

**What it is**: Smaller, faster successor to pybind11

**Performance**: ⭐⭐⭐⭐⭐ (Fastest C++ bindings)
- 4x smaller binaries than pybind11
- Lower overhead
- **Estimated overhead: 0.05-0.10 µs (~7-15%)**
- **Speedup vs CFFI: 3-6x faster**

**Similar pros/cons to pybind11, but requires C++17**

---

## Alternative 6: ctypes 🔴

**What it is**: Python's built-in FFI

**Performance**: ⭐⭐ (Slower than CFFI)
- More overhead than CFFI
- **Estimated overhead: 0.40-0.50 µs (~60-75%)**
- **SLOWER than CFFI!**

**Not recommended**

---

## Alternative 7: Mypyc 🟡

**What it is**: Compiles type-annotated Python to C extensions

**Performance**: ⭐⭐⭐ (Good for pure Python, not for C bindings)
- Only useful for Python code
- Can't improve C boundary crossing
- **Not applicable here**

---

## Comparison Table

| Alternative | Overhead | Speedup vs CFFI | Effort | Risk | LOC Change |
|-------------|----------|-----------------|--------|------|------------|
| **CFFI (current)** | 0.32 µs | 1.0x | - | - | 0 |
| **Python C API** | 0.05-0.10 µs | 3-6x | 1-2 weeks | High | ~1000+ |
| **Cython** | 0.08-0.12 µs | 2-4x | 2-3 days | Medium | ~300 |
| **PyO3 (Rust)** | 0.05-0.08 µs | 4-6x | 2-3 months | High | Full rewrite |
| **pybind11** | 0.08-0.15 µs | 2-4x | 1 week | Medium | ~500 |
| **nanobind** | 0.05-0.10 µs | 3-6x | 1 week | Medium | ~500 |
| **ctypes** | 0.40-0.50 µs | 0.6-0.8x | - | - | Slower! |

---

## Expected Results with Alternatives

### Current (CFFI)
- Total: 0.687 µs
- CFFI overhead: 0.320 µs (47%)
- C code: 0.367 µs (53%)

### With Python C API (Fastest)
- Total: **0.417-0.467 µs**
- Boundary overhead: 0.05-0.10 µs (11-21%)
- C code: 0.367 µs (79-89%)
- **Speedup: 1.5-1.6x faster overall**
- vs text2num (0.215 µs): Still 1.9-2.2x slower

### With Cython
- Total: **0.447-0.487 µs**
- Boundary overhead: 0.08-0.12 µs (16-24%)
- C code: 0.367 µs (76-84%)
- **Speedup: 1.4-1.5x faster overall**
- vs text2num: Still 2.1-2.3x slower

### With PyO3 (Full Rust Rewrite)
- Total: **0.215-0.300 µs** (if we match text2num's algorithm)
- Boundary overhead: 0.05-0.08 µs
- Rust code: 0.15-0.22 µs (if optimized like text2num)
- **Speedup: 2.3-3.2x faster overall**
- vs text2num: Competitive!
- **But requires full rewrite - why not just use text2num?**

---

## Recommendations

### 🏆 **Best Overall: Cython**

**Why**:
- ✓ Good performance (1.4-1.5x faster)
- ✓ Reasonable effort (2-3 days)
- ✓ Medium risk
- ✓ Can keep existing C code
- ✓ Easier to maintain than raw C API

**Expected result**: 0.447-0.487 µs (down from 0.687 µs)

### 🥈 **Best Performance: Python C API**

**Why**:
- ✓ Absolute fastest (1.5-1.6x faster)
- ✗ Lots of work (1-2 weeks)
- ✗ High risk (segfaults, memory leaks)
- ✗ Harder to maintain

**Expected result**: 0.417-0.467 µs

**Only worth it if**: You need maximum speed and have time to do it right

### 🥉 **Alternative: nanobind (if willing to use C++)**

**Why**:
- ✓ Very fast (1.5-1.6x faster)
- ✓ Safer than raw C API
- ✗ Requires C++ wrapper around C code
- ✗ 1 week effort

**Expected result**: 0.417-0.467 µs

### ❌ **NOT Worth It: PyO3 (Full Rust Rewrite)**

**Why**:
- ✓ Fastest possible (2-3x faster)
- ✗ **2-3 months of work**
- ✗ Complete rewrite
- ❓ **Why not just use text2num then?**

Only worth it if you need:
- Multi-language support (like text2num has)
- Comprehensive features (like OmNomNum has)
- Rust's memory safety
- All of the above in one library

---

## Combined with Other Optimizations

### Best Case Scenario: Cython + Fast Path + Caching

**Without cache (first call)**:
- CFFI overhead reduction: 0.320 → 0.080 µs (save 0.240 µs)
- Fast path for simple numbers: skip parser (save 0.250 µs)
- **New time: 0.687 - 0.240 - 0.250 = 0.197 µs**
- **2.9x faster than current**
- **1.1x faster than text2num!** 🔥

**With cache (repeated inputs)**:
- Direct cache lookup: ~0.070 µs
- **9.8x faster than current**
- **3.2x faster than text2num!** 🔥🔥

---

## Bottom Line

### Question 1: Caching Drawbacks?

**Drawbacks**:
1. ❌ **25% overhead for cache misses** (0.173 µs)
2. ❌ **Memory**: ~240 bytes per cached entry
3. ❌ **Stale data**: Must include config in cache key
4. ❌ **Not useful if inputs are always unique**

**Benefits**:
1. ✅ **9.6x faster for cache hits**
2. ✅ **Thread-safe** (with locking)
3. ✅ **Easy to implement** (5 minutes)

**Verdict**: Use if >10% of inputs are repeated

### Question 2: Something Faster than CFFI?

**Yes! Several options**:

1. **Python C API**: 3-6x faster bindings (but hard)
2. **Cython**: 2-4x faster bindings (good balance)
3. **PyO3**: 4-6x faster bindings (but needs Rust rewrite)
4. **nanobind**: 3-6x faster bindings (but needs C++)

**Best practical choice: Cython**
- 1.4-1.5x faster overall
- 2-3 days of work
- Keep existing C code
- Medium risk

**Best combined approach**:
- Cython (1.4x faster)
- + Fast path (2x faster)
- + Caching (10x faster for repeats)
- **= Up to 3x faster than text2num!** 🏆
