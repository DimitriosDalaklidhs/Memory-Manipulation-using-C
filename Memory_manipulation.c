/* bits_and_memory_c90.c
 *
 * OVERVIEW:
 *   Demonstrates common low-level C techniques:
 *     - bit flags and bitwise operators
 *     - masked writes to byte fields
 *     - parity via popcount
 *     - shadow buffers for safe data transformation
 *     - byte packing into 32-bit values
 *     - pointer arithmetic and bitfield packing
 *     - rotate-left operations
 *     - trailing-zero counting with UB avoidance
 *
 * ENTRY FLOW (main):
 *   1) Allocate and initialize a byte buffer.
 *   2) Manipulate permission-style flags.
 *   3) Populate buffer with diagnostic patterns.
 *   4) Compute parity for error detection.
 *   5) Create and transform a shadow copy.
 *   6) Pack bytes into integer representations.
 *   7) Demonstrate rotation and trailing-zero counting.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Flag bits packed into an unsigned integer */
#define FLAG_READ  (1u<<0)
#define FLAG_WRITE (1u<<1)
#define FLAG_EXEC  (1u<<2)

/* Brian Kernighan’s algorithm:
 * Counts the number of set bits by repeatedly clearing
 * the lowest set bit until the value becomes zero.
 */
unsigned count_bits(unsigned x) {
    unsigned c = 0;
    for (; x; x &= x - 1) {
        ++c;
    
	}
    return c;
}

/* Trailing-zero count.
 *
 * NOTE:
 *   __builtin_ctz(0) is undefined behavior, so the implementation
 *   must explicitly guard against zero input.
 */
static unsigned tzcount(unsigned x) {
#ifdef __GNUC__
    return __builtin_ctz(x ? x : 1u); /* defined result even for x==0 */
#else
    unsigned c = 0;
    if (x == 0) return 0;
    for (; (x & 1u) == 0u; x >>= 1) {
        ++c;
    }
    return c;
#endif
}

int main(void) {
    /* ------------------------------------------------------------
     * Setup and initialization
     * ------------------------------------------------------------ */
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

    /* ------------------------------------------------------------
     * Flag manipulation and controlled buffer updates
     * ------------------------------------------------------------ */

    /* Enable READ and WRITE permissions */
    flags |= FLAG_READ | FLAG_WRITE;

    /* Toggle EXEC permission */
    flags ^= FLAG_EXEC;

    /* Conditionally modify buffer based on WRITE permission */
    if (flags & FLAG_WRITE) {
        buf[0] |= (1u << 3);     /* set bit 3 */
    }

    /* Diagnostic alternating-bit pattern for visibility */
    buf[1] = 0xAAu;              /* 10101010 */

    /* Masked write:
     *   - Preserves bits not selected by mask
     *   - Updates only intended bit positions
     *
     * This prevents accidental corruption of unrelated fields.
     */
    buf[2] = (buf[2] & ~mask) | (val & mask);

    /* ------------------------------------------------------------
     * Parity check
     * ------------------------------------------------------------ */

    /* Parity is the least significant bit of the popcount.
     * Even parity => 0, odd parity => 1.
     */
    parity = count_bits(buf[1]) & 1u;

    /* ------------------------------------------------------------
     * Shadow buffer creation and transformation
     * ------------------------------------------------------------ */

    /* Create a working copy so transformations do not affect buf */
    memcpy(shadow, buf, n);

    /* Bitwise inversion of the shadow buffer */
    for (i = 0; i < n; ++i) {
        shadow[i] ^= 0xFFu;
    }

    /* ------------------------------------------------------------
     * Byte packing into a 32-bit accumulator
     * ------------------------------------------------------------ */

    /* Pack first four bytes of shadow in big-endian order.
     *
     * Subtlety:
     *   Shift-before-OR ensures correct byte placement.
     */
    acc = 0;
    for (i = 0; i < 4; ++i) {
        acc = (acc << 8) | shadow[i];
    }

    /* ------------------------------------------------------------
     * Pointer arithmetic and bitfield packing
     * ------------------------------------------------------------ */

    p = shadow;

    /* Insert lowest three flag bits into bits 0..2 of shadow[4].
     * Higher bits in the destination byte are preserved.
     */
    p[4] = (unsigned char)((p[4] & ~0x07u) | (flags & 0x07u));

    /* ------------------------------------------------------------
     * Rotate-left demonstration (32-bit)
     * ------------------------------------------------------------ */

    /* Rotate-left by 5 bits.
     *
     * Masking ensures a 32-bit result even when unsigned long
     * is wider than 32 bits on the target platform.
     */
    x = 0xDEADBEEFUL;
    x = ((x << 5) | (x >> (32 - 5))) & 0xFFFFFFFFUL;

    /* ------------------------------------------------------------
     * Trailing-zero count
     * ------------------------------------------------------------ */

    tz = tzcount(flags | 1u);

    /* ------------------------------------------------------------
     * Output and cleanup
     * ------------------------------------------------------------ */
    printf("flags=%u setbits=%u parity=%u acc=0x%08X x=0x%08lX\n",
           flags, count_bits(flags), parity, acc, x);
    printf("tz=%u\n", tz);

    free(buf);
    return 0;
}

