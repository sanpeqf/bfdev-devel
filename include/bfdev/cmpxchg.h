/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_CMPXCHG_H_
#define _BFDEV_CMPXCHG_H_

#include <bfdev/config.h>
#include <bfdev/asm/cmpxchg.h>

BFDEV_BEGIN_DECLS

#ifndef bfdev_xchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_xchg(bfdev_atomic_t *atomic, bfdev_atomic_t value)
{
    return bfdev_arch_xchg(atomic, value);
}
#endif

#ifndef bfdev_cmpxchg
static __bfdev_always_inline bfdev_atomic_t
bfdev_cmpxchg(bfdev_atomic_t *atomic, bfdev_atomic_t old, bfdev_atomic_t value)
{
    return bfdev_arch_cmpxchg(atomic, old, value);
}
#endif

BFDEV_END_DECLS

#endif /* _BFDEV_CMPXCHG_H_ */
