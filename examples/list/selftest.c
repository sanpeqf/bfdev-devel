/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <bfdev/macro.h>
#include <bfdev/list.h>

#define TEST_LOOP 10

struct test_node {
    bfdev_list_head_t list;
    unsigned long num;
};

struct test_pdata {
    struct test_node nodes[TEST_LOOP];
};

#define list_to_test(ptr) \
    bfdev_list_entry(ptr, struct test_node, list)

static long
list_test_sort(const bfdev_list_head_t *node1,
               const bfdev_list_head_t *node2, void *pdata)
{
    struct test_node *tnode1, *tnode2;

    tnode1 = list_to_test(node1);
    tnode2 = list_to_test(node2);

    if (tnode1->num == tnode2->num)
        return 0;

    return tnode1->num < tnode2->num ? -1 : 1;
}

static int
list_selftest(struct test_pdata *ldata)
{
    struct test_node *node, *nnode, *tnode;
    bfdev_list_head_t *list, *nlist, *tlist;
    unsigned int count;

    BFDEV_LIST_HEAD(test_head);

    for (count = 0; count < BFDEV_ARRAY_SIZE(ldata->nodes); ++count) {
        if (count % 1)
            bfdev_list_add_prev(&test_head, &ldata->nodes[count].list);
        else
            bfdev_list_add(&test_head, &ldata->nodes[count].list);
    }

    bfdev_list_sort(&test_head, list_test_sort, NULL);

    bfdev_list_for_each(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_continue(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_continue' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_list_for_each_from(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_from' test: %lu\n", node->num);
    }

    bfdev_list_for_each_reverse(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_reverse_continue(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_continue' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_list_for_each_reverse_from(list, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_from' test: %lu\n", node->num);
    }

    bfdev_list_for_each_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_continue_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_list_for_each_from_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_from_safe' test: %lu\n", node->num);
    }

    bfdev_list_for_each_reverse_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tlist = list;
    bfdev_list_for_each_reverse_continue_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_continue_safe' test: %lu\n", node->num);
    }

    list = tlist;
    bfdev_list_for_each_reverse_from_safe(list, nlist, &test_head) {
        node = list_to_test(list);
        printf("list 'list_for_each_reverse_from_safe' test: %lu\n", node->num);
    }

    bfdev_list_for_each_entry(node, &test_head, list) {
        printf("list 'list_for_each_entry' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_continue(node, &test_head, list) {
        printf("list 'list_for_each_entry_continue' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_list_for_each_entry_from(node, &test_head, list) {
        printf("list 'list_for_each_entry_from' test: %lu\n", node->num);
    }

    bfdev_list_for_each_entry_reverse(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_reverse_continue(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_continue' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_list_for_each_entry_reverse_from(node, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_from' test: %lu\n", node->num);
    }

    bfdev_list_for_each_entry_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
    }

    tnode = node;
    bfdev_list_for_each_entry_continue_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_list_for_each_entry_from_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_form_safe' test: %lu\n", node->num);
    }

    bfdev_list_for_each_entry_reverse_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_safe' test: %lu\n", node->num);
        if (node->num == TEST_LOOP / 2)
            break;
        bfdev_list_del(&node->list);
    }

    tnode = node;
    bfdev_list_for_each_entry_reverse_continue_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_continue_safe' test: %lu\n", node->num);
    }

    node = tnode;
    bfdev_list_for_each_entry_reverse_from_safe(node, nnode, &test_head, list) {
        printf("list 'list_for_each_entry_reverse_form_safe' test: %lu\n", node->num);
        bfdev_list_del(&node->list);
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    struct test_pdata *ldata;
    unsigned int count;
    int retval;

    ldata = malloc(sizeof(struct test_pdata));
    if (!ldata)
        return -1;

    for (count = 0; count < BFDEV_ARRAY_SIZE(ldata->nodes); ++count)
        ldata->nodes[count].num = count;

    retval = list_selftest(ldata);
    free(ldata);

    return retval;
}
