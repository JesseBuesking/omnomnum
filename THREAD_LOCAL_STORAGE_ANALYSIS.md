# Thread-Local Storage (TLS) for Parser State

## Your Question
"Is it possible to pre-allocate once per thread? Would that save on allocations? 
Is that an artifact of running a benchmark in a tight loop vs. a real world scenario 
of only allocating once at the start of a program's run?"

## Excellent Insight!

Yes, you've identified a key difference between:
1. **Benchmark scenario**: Tight loop, same thread, repeated calls
2. **Real-world scenario**: Varies widely depending on use case

## Current Design: Caller-Managed State

```c
// Current API
ParserState st;
initParserState(&st);
for (...) {
    normalize(text, len, &st);
    resetParserState(&st);
}
freeParserState(&st);
```

**Pros:**
- Caller controls lifecycle
- Works for single-threaded or thread-per-request
- No hidden global state
- Library is thread-safe

**Cons:**
- Caller must remember to init/reset/free
- Easy to misuse (forget reset, memory leak, etc.)

## Option 1: Thread-Local Storage

```c
__thread static ParserState *tls_state = NULL;

sds normalize_tls(const char *data, size_t len) {
    if (!tls_state) {
        tls_state = malloc(sizeof(ParserState));
        initParserState(tls_state);
    }
    normalize(data, len, tls_state);
    sds result = tls_state->result;
    tls_state->result = NULL;  // Transfer ownership
    resetParserState(tls_state);
    return result;
}
```

**Pros:**
- Zero allocation overhead after first call per thread
- Simple API for callers
- Automatically handles thread safety

**Cons:**
- Not portable (TLS syntax varies: `__thread`, `thread_local`, `_Thread_local`)
- Memory persists until thread death
- Can't customize ParserState flags per call
- Hidden global state (harder to reason about)

## Option 2: Hybrid API

```c
// Simple API for one-offs
sds normalize_simple(const char *data, size_t len);

// Advanced API with caller-managed state
void normalize(const char *data, size_t len, ParserState *st);
```

## Benchmark vs Real-World Trade-offs

### Benchmark Scenario (Current Tests)
- **Pattern**: Single thread, tight loop, same ParserState
- **Benefit from**: Buffer reuse (OPT1), capacity pre-allocation (OPT2)
- **Current optimizations work perfectly**

### Real-World Scenarios

**1. Web Server (Thread-per-request)**
```c
void handle_request(Request *req) {
    ParserState st;  // Stack allocation
    initParserState(&st);
    normalize(req->body, req->len, &st);
    // ...
    freeParserState(&st);
}
```
- **Allocation pattern**: One init/free per request
- **OPT1/OPT2 don't help much** (only 1 normalize() call)
- **TLS would help significantly** (zero alloc after first request on thread)

**2. CLI Tool (Single parse)**
```c
int main() {
    ParserState st;
    initParserState(&st);
    normalize(input, len, &st);
    printf("%s\n", st.result);
    freeParserState(&st);
}
```
- **Allocation pattern**: One-time
- **OPT1/OPT2 don't matter** (single call)
- **TLS wouldn't help** (only one call anyway)

**3. Stream Processor (Many calls, same thread)**
```c
while (line = read_line()) {
    normalize(line, strlen(line), &st);
    resetParserState(&st);
}
```
- **Allocation pattern**: Many calls, same state
- **OPT1/OPT2 help significantly** (exactly what we optimized for)
- **TLS not needed** (already reusing state)

## Recommendation

### For Library Maintainer (You)
1. **Keep current API** - caller-managed state is most flexible
2. **Optionally add TLS convenience wrapper** for simple use cases
3. **Document usage patterns** clearly in README

### For Benchmark Validity
Your benchmarks ARE representative of:
- Stream processing
- Batch normalization
- Any scenario with repeated calls

Your benchmarks are NOT representative of:
- One-shot CLI tools
- Request-per-thread web servers (unless using TLS)

## Would TLS Help Your Benchmarks?

**No!** Benchmarks already reuse state optimally. TLS would add:
- TLS lookup overhead
- Less flexibility (can't customize flags per test)
- No performance benefit (already doing best-case reuse)

## Conclusion

Your optimization strategy (OPT1+OPT2) is **correct** for the benchmark scenario AND
for real-world stream processing scenarios. TLS would be a nice-to-have convenience
wrapper for request-per-thread scenarios, but wouldn't improve benchmark performance.

The benchmarks accurately measure the library's performance for its primary use case:
**repeated normalization calls on the same thread with state reuse**.
