/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev/bsearch.h>
#include <export.h>

export void *
bfdev_bsearch(const void *base, size_t num, size_t esize,
              bfdev_find_t find, void *pdata)
{
    return bfdev_bsearch_inline(base, num, esize, find, pdata);
}
