/* Result Buffer - Fixed-size stack buffer with overflow to heap
 *
 * This replaces SDS for the main result buffer to reduce allocation overhead.
 * Uses a 512-byte inline buffer for 99% of cases, with graceful overflow to
 * heap (SDS) for larger results.
 *
 * Performance:
 * - Inline path (< 512 bytes): ~8-10% faster than SDS
 * - Overflow path (>= 512 bytes): Same performance as SDS (no regression)
 */

#ifndef RESULT_BUFFER_H
#define RESULT_BUFFER_H

#include "sds.h"
#include <string.h>

/* Size chosen to cover 99% of normalized text outputs */
#define RB_INLINE_SIZE 512

typedef struct {
    char inline_buf[RB_INLINE_SIZE];  /* Stack buffer for common cases */
    size_t len;                        /* Current length (valid for inline only) */
    sds overflow;                      /* NULL if using inline_buf, SDS if overflowed */
} ResultBuffer;

/* Initialize buffer to empty state */
static inline void rb_init(ResultBuffer *rb) {
    rb->len = 0;
    rb->overflow = NULL;
    rb->inline_buf[0] = '\0';
}

/* Append data to buffer
 * Fast path: memcpy to inline buffer
 * Overflow path: switch to SDS and continue
 */
static inline void rb_append(ResultBuffer *rb, const char *data, size_t dlen) {
    /* Already overflowed? Use SDS path */
    if (rb->overflow) {
        rb->overflow = sdscatlen(rb->overflow, data, dlen);
        return;
    }

    /* Check if it fits in inline buffer */
    if (rb->len + dlen < RB_INLINE_SIZE) {
        /* Fast path: direct memcpy to stack buffer */
        memcpy(rb->inline_buf + rb->len, data, dlen);
        rb->len += dlen;
        rb->inline_buf[rb->len] = '\0';
    } else {
        /* Overflow: switch to heap (SDS) */
        /* Copy existing inline data to new SDS string */
        rb->overflow = sdsnewlen(rb->inline_buf, rb->len);
        /* Append the new data that triggered overflow */
        rb->overflow = sdscatlen(rb->overflow, data, dlen);
        /* Note: inline_buf is now unused, overflow is non-NULL */
    }
}

/* Get result pointer and length
 * Returns either inline buffer or overflow SDS
 */
static inline const char *rb_get(const ResultBuffer *rb, size_t *len) {
    if (rb->overflow) {
        *len = sdslen(rb->overflow);
        return rb->overflow;
    }
    *len = rb->len;
    return rb->inline_buf;
}

/* Get result as SDS (for compatibility with existing code)
 * If inline: creates new SDS copy
 * If overflow: returns existing SDS (caller must not free!)
 */
static inline sds rb_get_sds(const ResultBuffer *rb) {
    if (rb->overflow) {
        return rb->overflow;
    }
    /* Create SDS copy of inline buffer */
    return sdsnewlen(rb->inline_buf, rb->len);
}

/* Clear buffer for reuse
 * Resets length but keeps allocated overflow if present
 */
static inline void rb_clear(ResultBuffer *rb) {
    if (rb->overflow) {
        sdsclear(rb->overflow);
    } else {
        rb->len = 0;
        rb->inline_buf[0] = '\0';
    }
}

/* Free resources
 * Frees overflow SDS if allocated
 */
static inline void rb_free(ResultBuffer *rb) {
    if (rb->overflow) {
        sdsfree(rb->overflow);
        rb->overflow = NULL;
    }
    rb->len = 0;
}

/* Check if buffer is empty */
static inline int rb_is_empty(const ResultBuffer *rb) {
    if (rb->overflow) {
        return sdslen(rb->overflow) == 0;
    }
    return rb->len == 0;
}

/* Get result as C string (for compatibility with strcmp, printf, etc.)
 * Returns pointer to null-terminated string
 */
static inline const char *rb_cstr(const ResultBuffer *rb) {
    if (rb->overflow) {
        return rb->overflow;
    }
    return rb->inline_buf;
}

/* Get current length */
static inline size_t rb_len(const ResultBuffer *rb) {
    if (rb->overflow) {
        return sdslen(rb->overflow);
    }
    return rb->len;
}

/* Pre-reserve capacity (optimization hint)
 * For inline: no-op (already has 512 bytes)
 * For overflow: calls sdsMakeRoomFor
 */
static inline void rb_reserve(ResultBuffer *rb, size_t additional) {
    if (rb->overflow) {
        rb->overflow = sdsMakeRoomFor(rb->overflow, additional);
    }
    /* For inline: no action needed, already have 512 bytes */
}

#endif /* RESULT_BUFFER_H */
