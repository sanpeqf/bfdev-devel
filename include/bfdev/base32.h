/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BASE32_H_
#define _BFDEV_BASE32_H_

#include <bfdev/config.h>
#include <bfdev/errno.h>
#include <bfdev/math.h>

BFDEV_BEGIN_DECLS

static inline size_t
bfdev_base32_encode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 5) * 8 + 1;
}

static inline size_t
bfdev_base32_decode_length(size_t size)
{
    return BFDEV_DIV_ROUND_UP(size, 8) * 5;
}

extern void bfdev_base32_encode(void *buff, const void *data, size_t size);
extern int bfdev_base32_decode(void *buff, const void *data, size_t size);

BFDEV_END_DECLS

#endif /* _BFDEV_BASE32_H_ */
