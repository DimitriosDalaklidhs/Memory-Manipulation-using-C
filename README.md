**BITS AND MEMORY — C90 DEMONSTRATION**

This program is a compact ANSI C (C90) example focused on low-level bit manipulation, memory handling, and pointer arithmetic. It is intended as a learning and reference file for classic systems programming techniques.

**File Overview**

File name: bits_and_memory_c90.c
Language standard: ANSI C (C90)

The code demonstrates portable low-level logic, manual memory management, bitwise operations, and limited compiler-aware optimizations.

What the Program Demonstrates

The program uses integer bit flags, masked writes, parity checks, and rotate-left operations. It includes Brian Kernighan’s algorithm for counting set bits and a trailing zero count implementation that uses a GCC builtin when available, with a portable fallback otherwise.

Memory usage includes dynamic allocation with malloc and free, zero initialization with memset, shadow buffer copying, byte inversion, and accumulation of multiple bytes into a 32-bit value. Pointer arithmetic is used to directly manipulate and pack bits into specific memory locations.

**Key Concepts**

Permission-style flags are defined using bit shifts and stored in a single integer. Population count is implemented using Kernighan’s technique, where each iteration clears the lowest set bit. Trailing zero count avoids undefined behavior and remains portable across compilers.

Masked writes allow selective modification of bits without affecting surrounding data. A rotate-left operation is implemented manually using shifts and masking, assuming a 32-bit word size.
