/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_BLOOM_H_
#define _BFDEV_BLOOM_H_

#include <bfdev/config.h>
#include <bfdev/stdbool.h>
#include <bfdev/errno.h>
#include <bfdev/allocator.h>

BFDEV_BEGIN_DECLS

typedef unsigned int (*bfdev_bloom_hash_t)
(const void *key, void *pdata);

struct bfdev_bloom {
    const struct bfdev_alloc *alloc;
    bfdev_bloom_hash_t hash;
    void *pdata;

    unsigned int capacity;
    unsigned long bitmap[];
};

/**
 * bfdev_bloom_peek() - peek an object from a bloom filter.
 * @bloom: bloom filter pointer.
 * @key: object pointer.
 *
 * @return: object value.
 */
extern bool
bfdev_bloom_peek(struct bfdev_bloom *bloom, void *key);

/**
 * bfdev_bloom_push() - push an object from a bloom filter.
 * @bloom: bloom filter pointer.
 * @key: object pointer to push.
 *
 * @return: object value before push.
 */
extern bool
bfdev_bloom_push(struct bfdev_bloom *bloom, void *key);

/**
 * bfdev_bloom_clear() - clean an object from a bloom filter.
 * @bloom: bloom filter pointer.
 * @key: object pointer to clear.
 *
 * @return: object value before clear.
 */
extern bool
bfdev_bloom_clear(struct bfdev_bloom *bloom, void *key);

/**
 * bfdev_bloom_flush() - flush the entire bloom filter.
 * @bloom: bloom filter pointer.
 */
extern void
bfdev_bloom_flush(struct bfdev_bloom *bloom);

/**
 * bfdev_bloom_create() - creat a bloom filter.
 * @capacity: capacity size of bloom filter.
 * @hash: object hash callback function.
 * @pdata: private data pointer of @hash.
 */
extern struct bfdev_bloom *
bfdev_bloom_create(const struct bfdev_alloc *alloc, unsigned int capacity,
                   bfdev_bloom_hash_t hash, void *pdata);

/**
 * bfdev_bloom_destory() - destory a bloom filter.
 * @bloom: bloom filter pointer.
 */
extern void
bfdev_bloom_destory(struct bfdev_bloom *bloom);

BFDEV_END_DECLS

#endif /* _BFDEV_BLOOM_H_ */
