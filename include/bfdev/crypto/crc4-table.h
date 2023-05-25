/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CRYPTO_CRC4_TABLE_H_
#define _BFDEV_CRYPTO_CRC4_TABLE_H_

#include <bfdev/config.h>
#include <bfdev/stdint.h>
#include <bfdev/stddef.h>
#include <bfdev/minmax.h>
#include <bfdev/bits.h>

#ifdef __cplusplus
extern "C" {
#endif

static const uint8_t bfdev_crc4_table[256] = {
    0x00, 0x07, 0x0e, 0x09,
    0x0b, 0x0c, 0x05, 0x02,
    0x01, 0x06, 0x0f, 0x08,
    0x0a, 0x0d, 0x04, 0x03,
};

static inline uint8_t
bfdev_crc4_byte(uint8_t crc, const uint8_t data)
{
    crc = bfdev_crc4_table[crc ^ (data >> 4)];
    return bfdev_crc4_table[crc ^ (data & 0xf)];
}

static inline uint8_t
bfdev_crc4_inline(const uint8_t *src, size_t bits, uint8_t crc)
{
    unsigned int count;
    uint8_t value;

    for (; bits >= 64; bits -= 64) {
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
        crc = bfdev_crc4_byte(crc, *src++);
    }

    for (; (count = bfdev_min(bits, 8)); bits -= count) {
        value = BFDEV_BIT_LOW_MASK(bits) & *src++;
        crc = bfdev_crc4_byte(crc, value);
    }

    return crc;
}

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_CRYPTO_CRC4_TABLE_H_ */
