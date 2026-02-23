# Bits and Memory — C90 Demonstration

A compact ANSI C (C90) program focused on low-level bit manipulation, memory handling, and pointer arithmetic. Intended as a learning and reference file for classic systems programming techniques.

---

## File Overview

| Field | Detail |
|---|---|
| Filename | `bits_and_memory_c90.c` |
| Standard | ANSI C (C90) |

---

## What It Demonstrates

### Bit Manipulation
- Integer bit flags, masked writes, and parity checks
- Rotate-left operation (manual implementation, assumes 32-bit word size)
- Brian Kernighan's algorithm for counting set bits (each iteration clears the lowest set bit)
- Trailing zero count — uses a GCC builtin when available, with a portable fallback otherwise

### Memory Management
- Dynamic allocation with `malloc` and `free`
- Zero initialization with `memset`
- Shadow buffer copying and byte inversion
- Accumulating multiple bytes into a 32-bit value

### Pointer Arithmetic
- Direct bit manipulation and packing into specific memory locations

---

## Key Concepts

**Permission-style flags** are defined using bit shifts and stored in a single integer.

**Population count** is implemented using Kernighan's technique — each iteration clears the lowest set bit until none remain.

**Trailing zero count** avoids undefined behavior and stays portable across compilers.

**Masked writes** allow selective bit modification without touching surrounding data.

**Rotate-left** is implemented manually via shifts and masking, assuming a 32-bit word size.

---

> Some of the concepts demonstrated here are directly relevant to modern cloud infrastructure and low-level systems troubleshooting.
