/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/rbtree.h>
#include <export.h>

export bool
bfdev_rb_debug_link_check(struct bfdev_rb_node *parent, struct bfdev_rb_node **link, struct bfdev_rb_node *node)
{
    if (unlikely(*link == node)) {
        fprintf(stderr, "rb_insert corruption (%p) *link should not be node (%p)\n",
        link, node);
        return false;
    }

    return true;
}

export bool
bfdev_rb_debug_delete_check(struct bfdev_rb_node *node)
{
    if (unlikely(node->left == BFDEV_POISON_RBNODE1)) {
        fprintf(stderr, "rb_delete corruption (%p) node->left should not be BFDEV_POISON_RBNODE1 (%p)\n",
        node, BFDEV_POISON_RBNODE1);
        return false;
    }

    if (unlikely(node->right == BFDEV_POISON_RBNODE2)) {
        fprintf(stderr, "rb_delete corruption (%p) node->right should not be BFDEV_POISON_RBNODE2 (%p)\n",
        node, BFDEV_POISON_RBNODE2);
        return false;
    }

    if (unlikely(node->parent == BFDEV_POISON_RBNODE3)) {
        fprintf(stderr, "rb_delete corruption (%p) node->parent should not be BFDEV_POISON_RBNODE3 (%p)\n",
        node, BFDEV_POISON_RBNODE3);
        return false;
    }

    return true;
}
