/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_ASM_GENERIC_BITOPS_H_
#define _BFDEV_ASM_GENERIC_BITOPS_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stdbool.h>
#include <bfdev/bits.h>
#include <bfdev/asm/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef bfdev_arch_bit_clr
static __always_inline void
bfdev_arch_bit_clr(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr &= ~BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_set
static __always_inline void
bfdev_arch_bit_set(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr |= BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_flip
static __always_inline void
bfdev_arch_bit_flip(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    *addr ^= BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_change
static __always_inline void
bfdev_arch_bit_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        *addr |= BFDEV_BIT(bit);
    else
        *addr &= ~BFDEV_BIT(bit);
}
#endif

#ifndef bfdev_arch_bit_test
static __always_inline bool
bfdev_arch_bit_test(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    return !!(*addr & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_clr
static __always_inline bool
bfdev_arch_bit_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr &= ~BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_set
static __always_inline bool
bfdev_arch_bit_test_set(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr |= BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_flip
static __always_inline bool
bfdev_arch_bit_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    *addr ^= BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_test_change
static __always_inline bool
bfdev_arch_bit_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = *addr;
    if (val)
        *addr |= BFDEV_BIT(bit);
    else
        *addr &= ~BFDEV_BIT(bit);
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_clr
static __always_inline void
bfdev_arch_bit_atomic_clr(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_set
static __always_inline void
bfdev_arch_bit_atomic_set(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_flip
static __always_inline void
bfdev_arch_bit_atomic_flip(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    bfdev_arch_atomic_xor((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_change
static __always_inline void
bfdev_arch_bit_atomic_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        bfdev_arch_atomic_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    else
        bfdev_arch_atomic_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test
static __always_inline bool
bfdev_arch_bit_atomic_test(volatile unsigned long *addr, unsigned int bit)
{
    addr += BFDEV_BITS_WORD(bit);
    return !!(bfdev_arch_atomic_read((bfdev_atomic_t *)addr) & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_clr
static __always_inline bool
bfdev_arch_bit_atomic_test_clr(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    if ((BFDEV_READ_ONCE(*addr) & BFDEV_BIT(bit)) == 0) return 0;
    old = bfdev_arch_atomic_fetch_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_set
static __always_inline bool
bfdev_arch_bit_atomic_test_set(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    if ((BFDEV_READ_ONCE(*addr) & BFDEV_BIT(bit)) != 0) return 1;
    old = bfdev_arch_atomic_fetch_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_flip
static __always_inline bool
bfdev_arch_bit_atomic_test_flip(volatile unsigned long *addr, unsigned int bit)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    old = bfdev_arch_atomic_fetch_xor((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

#ifndef bfdev_arch_bit_atomic_test_change
static __always_inline bool
bfdev_arch_bit_atomic_test_change(volatile unsigned long *addr, unsigned int bit, bool val)
{
    unsigned long old;
    addr += BFDEV_BITS_WORD(bit);
    if (val)
        old = bfdev_arch_atomic_fetch_or((bfdev_atomic_t *)addr, BFDEV_BIT(bit));
    else
        old = bfdev_arch_atomic_fetch_and((bfdev_atomic_t *)addr, ~BFDEV_BIT(bit));
    return !!(old & BFDEV_BIT(bit));
}
#endif

/**
 * bfdev_arch_ffsuf - find first set bit in word (unsafe).
 * @val: The word to search.
 * Undefined if no bit exists, so code should check against 0 first..
 */
#ifndef bfdev_arch_ffsuf
static __always_inline unsigned int
bfdev_arch_ffsuf(unsigned long value)
{
    unsigned int shift = 0;

#if BFDEV_BITS_PER_LONG == 64
    if ((value & 0xffffffff) == 0) {
        shift += 32;
        value >>= 32;
    }
#endif

    if ((value & 0xffff) == 0) {
        shift += 16;
        value >>= 16;
    }

    if ((value & 0xff) == 0) {
        shift += 8;
        value >>= 8;
    }

    if ((value & 0xf) == 0) {
        shift += 4;
        value >>= 4;
    }

    if ((value & 0x3) == 0) {
        shift += 2;
        value >>= 2;
    }

    if ((value & 0x1) == 0) {
        shift += 1;
    }

    return shift;
}
#endif

/*
 * bfdev_arch_flsuf: find last set bit in word (unsafe).
 * @val: The word to search.
 * Undefined if no set bit exists, so code should check against 0 first..
 */
#ifndef bfdev_arch_flsuf
static __always_inline unsigned int
bfdev_arch_flsuf(unsigned long value)
{
    unsigned int shift = BFDEV_BITS_PER_LONG - 1;

#if BFDEV_BITS_PER_LONG == 64
    if (!(value & (~0ul << 32))) {
        shift -= 32;
        value <<= 32;
    }
#endif

    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 16)))) {
        shift -= 16;
        value <<= 16;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 8)))) {
        shift -= 8;
        value <<= 8;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 4)))) {
        shift -= 4;
        value <<= 4;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 2)))) {
        shift -= 2;
        value <<= 2;
    }
    if (!(value & (~0ul << (BFDEV_BITS_PER_LONG - 1))))
        shift -= 1;

    return shift;
}
#endif

/*
 * bfdev_arch_ffzuf - find first zero in word (unsafe).
 * @word: The word to search.
 * Undefined if no zero exists, so code should check against ~0UL first..
 */
#ifndef bfdev_arch_ffzuf
static __always_inline unsigned int
bfdev_arch_ffzuf(unsigned long value)
{
    return bfdev_arch_ffsuf(~value);
}
#endif

/*
 * bfdev_arch_flzuf - find last zero in word (unsafe).
 * @word: The word to search.
 * Undefined if no zero exists, so code should check against ~0UL first..
 */
#ifndef bfdev_arch_flzuf
static __always_inline unsigned int
bfdev_arch_flzuf(unsigned long value)
{
    return bfdev_arch_flsuf(~value);
}
#endif

/**
 * bfdev_arch_ffs - find first bit set.
 * @value: the word to search.
 */
#ifndef bfdev_arch_ffs
static __always_inline unsigned int
bfdev_arch_ffs(unsigned long value)
{
    if (!value)
        return 0;
    return bfdev_arch_ffsuf(value) + 1;
}
#endif

/**
 * bfdev_arch_fls - find last zero in word.
 * @value: the word to search.
 */
#ifndef bfdev_arch_fls
static __always_inline unsigned int
bfdev_arch_fls(unsigned long value)
{
    if (!value)
        return 0;
    return bfdev_arch_flsuf(value) + 1;
}
#endif

/*
 * bfdev_arch_ffz - find first zero in word.
 * @value: The word to search.
 */
#ifndef bfdev_arch_ffz
static __always_inline unsigned int
bfdev_arch_ffz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    return bfdev_arch_ffs(~value);
}
#endif

/*
 * bfdev_arch_flz - find last zero in word.
 * @value: The word to search.
 */
#ifndef bfdev_arch_flz
static __always_inline unsigned int
bfdev_arch_flz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    return bfdev_arch_fls(~value);
}
#endif

/*
 * bfdev_arch_ctz - returns the number of trailing 0-bits in value.
 * @value: The word to search.
 */
#ifndef bfdev_arch_ctz
static __always_inline unsigned int
bfdev_arch_ctz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    if (!value)
        return BFDEV_BITS_PER_LONG;
    return bfdev_arch_ffsuf(value);
}
#endif

/*
 * bfdev_arch_clz - returns the number of leading 0-bits in value.
 * @value: The word to search.
 */
#ifndef bfdev_arch_clz
static __always_inline unsigned int
bfdev_arch_clz(unsigned long value)
{
    if (value == ULONG_MAX)
        return 0;
    if (!value)
        return BFDEV_BITS_PER_LONG;
    return BFDEV_BITS_PER_LONG - 1 - bfdev_arch_flsuf(value);
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_ASM_GENERIC_BITOPS_H_ */
