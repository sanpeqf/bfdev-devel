/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev/macro.h>
#include <bfdev/matrix.h>

#define TEST_BASE 1
#define TEST_MAXN 46

static bfdev_matrix_t
matrix_power = {
    .row = 2, .col = 2,
    .values = {
        0, 1,
        1, 0,
    },
};

static bfdev_matrix_t
matrix_fibonacci = {
    .row = 2, .col = 2,
    .values = {
        1, 1,
        1, 0,
    },
};

static bfdev_matrix_t *
power(const bfdev_matrix_t *var, unsigned int pow)
{
    bfdev_matrix_t *result;
    bfdev_matrix_t *tmp;

    result = bfdev_matrix_copy(NULL, &matrix_power);
    var = bfdev_matrix_copy(NULL, var);

    while (pow) {
        if (pow & 1) {
            tmp = bfdev_matrix_mul(NULL, result, var);
            if (bfdev_unlikely(!tmp))
                return NULL;

            bfdev_matrix_destory(NULL, result);
            result = tmp;
        }

        tmp = bfdev_matrix_mul(NULL, var, var);
        if (bfdev_unlikely(!tmp))
            return NULL;

        bfdev_matrix_destory(NULL, var);
        var = tmp;
        pow >>= 1;
    }

    bfdev_matrix_destory(NULL, var);
    return result;
}

int main(int argc, const char *argv[])
{
    bfdev_matrix_t *result;
    unsigned int count;

    for (count = TEST_BASE; count <= TEST_MAXN; ++count) {
        result = power(&matrix_fibonacci, count);
        printf("matrix fibonacci %02u: %ld\n", count, result->values[0]);
        bfdev_matrix_destory(NULL, result);
    }

    return 0;
}
