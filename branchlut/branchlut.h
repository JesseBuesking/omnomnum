#ifndef BRANCHLUT_H
#define BRANCHLUT_H

#include <stdint.h>

// Branching for different cases (forward)
// Use lookup table of two digits

int u32toa_branchlut(uint32_t value, char* buffer);
int i32toa_branchlut(int32_t value, char* buffer);
int u64toa_branchlut(uint64_t value, char* buffer);
int i64toa_branchlut(int64_t value, char* buffer);

#endif // BRANCHLUT_H
