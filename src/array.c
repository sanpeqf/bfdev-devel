/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 ffashion <helloworldffashion@gmail.com>
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/array.h>
#include <export.h>

export void *
bfdev_array_push(struct bfdev_array *array, unsigned int num)
{
    const struct bfdev_alloc *alloc = array->alloc;
    unsigned int nalloc, index;
    void *data;

    nalloc = array->index + num;
    if (nalloc > array->capacity) {
        nalloc = bfdev_max(nalloc << 1, BFDEV_ARRAY_MSIZE);

        data = bfdev_realloc(alloc, array->data, nalloc * array->cells);
        if (bfdev_unlikely(!data))
            return NULL;

        array->data = data;
        array->capacity = nalloc;
    }

    index = array->index;
    array->index += num;

    return bfdev_array_data(array, index);
}

export void *
bfdev_array_pop(struct bfdev_array *array, unsigned int num)
{
    unsigned int index;

    if (!num || array->index < num)
        return NULL;

    index = array->index - num;
    array->index = index;

    return bfdev_array_data(array, index);
}

export void
bfdev_array_release(struct bfdev_array *array)
{
    const struct bfdev_alloc *alloc = array->alloc;

    array->capacity = 0;
    array->index = 0;

    bfdev_free(alloc, array->data);
    array->data = NULL;
}
