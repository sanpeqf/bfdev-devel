/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_
#define _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/bits.h>
#include <bfdev/attributes.h>

BFDEV_BEGIN_DECLS

#define bfdev_arch_flsuf bfdev_arch_flsuf
static __always_inline unsigned int bfdev_arch_flsuf(unsigned long value)
{
	return BFDEV_BITS_PER_LONG - 1 - __builtin_clzl(value);
}

BFDEV_END_DECLS

#endif /* _BFDEV_ASM_GENERIC_BUILTIN_FLSUF_H_ */
