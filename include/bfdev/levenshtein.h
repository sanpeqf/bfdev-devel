/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LEVENSHTEIN_H_
#define _BFDEV_LEVENSHTEIN_H_

#include <bfdev/stdint.h>
#include <bfdev/stddef.h>
#include <bfdev/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bfdev_levenshtein_len - Calculation string levenshtein edit distance.
 * @str1: string1 to calculation distance.
 * @str2: string2 to calculation distance.
 * @len1: the length of @str1.
 * @len2: the length of @str2.
 * @s: substitution distance reward.
 * @w: swap distance reward.
 * @a: addition distance reward.
 * @d: deletion distance reward.
 */
extern unsigned int
bfdev_levenshtein_len(const struct bfdev_alloc *alloc,
                      const char *str1, const char *str2,
                      size_t len1, size_t len2,
                      unsigned int s, unsigned int w,
                      unsigned int a, unsigned int d);

/**
 * bfdev_levenshtein - Calculation string levenshtein edit distance.
 * @str1: string1 to calculation distance.
 * @str2: string2 to calculation distance.
 * @s: substitution distance reward.
 * @w: swap distance reward.
 * @a: addition distance reward.
 * @d: deletion distance reward.
 */
extern unsigned int
bfdev_levenshtein(const struct bfdev_alloc *alloc,
                  const char *str1, const char *str2,
                  unsigned int s, unsigned int w,
                  unsigned int a, unsigned int d);

#ifdef __cplusplus
}
#endif

#endif /* _BFDEV_LEVENSHTEIN_H_ */
