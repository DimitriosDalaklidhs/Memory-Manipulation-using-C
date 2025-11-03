/* bits_and_memory_c90.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAG_READ  (1u<<0)
#define FLAG_WRITE (1u<<1)
#define FLAG_EXEC  (1u<<2)

/* popcount (Brian Kernighan) */
static unsigned count_bits(unsigned x) {
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

/* trailing zero count with GCC builtin fallback */
static unsigned tzcount(unsigned x) {
#ifdef __GNUC__
    return __builtin_ctz(x ? x : 1u); /* avoid UB on 0; returns 0 for x==0 */
#else
    unsigned c = 0;
    if (x == 0) return 0;
    while ((x & 1u) == 0u) { ++c; x >>= 1; }
    return c;
#endif
}

int main(void) {
    unsigned n = 16;
    unsigned flags = 0;
    unsigned char *buf = (unsigned char*)malloc(n);
    unsigned mask = 0xF0u, val = 0x3Cu;
    unsigned parity, acc, i;
    unsigned char shadow[16];
    unsigned char *p;
    unsigned long x;
    unsigned tz;

    if (!buf) return 1;
    memset(buf, 0, n);

    /* flags & basic bit ops */
    flags |= FLAG_READ | FLAG_WRITE;          /* set R/W */
    flags ^= FLAG_EXEC;                       /* toggle X */
    if (flags & FLAG_WRITE) buf[0] |= (1u << 3); /* set bit3 */
    buf[1] = 0xAAu;                           /* 10101010 pattern */
    buf[2] = (buf[2] & ~mask) | (val & mask); /* masked write */

    /* parity (LSB of popcount) */
    parity = count_bits(buf[1]) & 1u;

    /* shadow copy + invert */
    memcpy(shadow, buf, n);
    for (i = 0; i < n; ++i) shadow[i] ^= 0xFFu;

    /* accumulate first 4 bytes into a 32-bit value */
    acc = 0;
    for (i = 0; i < 4; ++i) acc = (acc << 8) | shadow[i];

    /* pointer math: pack flags’ 3 LSBs into bits 0..2 of shadow[4] */
    p = shadow;
    p[4] = (unsigned char)((p[4] & ~0x07u) | (flags & 0x07u));

    /* rotate-left 5 on 32-bit value (assumes 32-bit unsigned long on your toolchain) */
    x = 0xDEADBEEFUL;
    x = ((x << 5) | (x >> (32 - 5))) & 0xFFFFFFFFUL;

    tz = tzcount(flags | 1u);

    printf("flags=%u setbits=%u parity=%u acc=0x%08X x=0x%08lX\n",
           flags, count_bits(flags), parity, acc, x);
    printf("tz=%u\n", tz);

    free(buf);
    return 0;
}

