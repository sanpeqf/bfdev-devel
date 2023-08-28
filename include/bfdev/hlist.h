/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_HLIST_H_
#define _BFDEV_HLIST_H_

#include <bfdev/config.h>
#include <bfdev/stddef.h>
#include <bfdev/stdbool.h>
#include <bfdev/poison.h>
#include <bfdev/container.h>

BFDEV_BEGIN_DECLS

struct bfdev_hlist_node {
    struct bfdev_hlist_node *next;
    struct bfdev_hlist_node **pprev;
};

struct bfdev_hlist_head {
    struct bfdev_hlist_node *node;
};

#define BFDEV_HLIST_HEAD_STATIC \
    {NULL}

#define BFDEV_HLIST_HEAD_INIT \
    (struct bfdev_hlist_head) BFDEV_HLIST_HEAD_STATIC

#define BFDEV_HLIST_HEAD(name) \
    struct bfdev_hlist_head name = BFDEV_HLIST_HEAD_INIT

#ifdef BFDEV_DEBUG_HLIST
extern bool bfdev_hlist_debug_head_add_check(struct bfdev_hlist_head *head, struct bfdev_hlist_node *new);
extern bool bfdev_hlist_debug_next_add_check(struct bfdev_hlist_node *next, struct bfdev_hlist_node *new);
extern bool bfdev_hlist_debug_prev_add_check(struct bfdev_hlist_node *prev, struct bfdev_hlist_node *new);
extern bool bfdev_hlist_debug_del_check(struct bfdev_hlist_node *node);
#endif

/**
 * bfdev_hlist_head_init - initialize a bfdev_hlist_head structure.
 * @head: hlist_head structure to be initialized.
 */
static inline void
bfdev_hlist_head_init(struct bfdev_hlist_head *head)
{
    head->node = NULL;
}

/**
 * bfdev_hlist_node_init - initialize a bfdev_hlist_node structure.
 * @node: bfdev_hlist_node structure to be initialized.
 */
static inline void
bfdev_hlist_node_init(struct bfdev_hlist_node *node)
{
    node->pprev = NULL;
    node->next = NULL;
}

/**
 * bfdev_hlist_head_add - add a new entry at the beginning of the hlist.
 * @head: hlist head to add it after.
 * @new: new entry to be added.
 */
static inline void
bfdev_hlist_head_add(struct bfdev_hlist_head *head, struct bfdev_hlist_node *new)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_debug_head_add_check(head, new)))
        return;
#endif

    new->next = head->node;
    new->pprev = &head->node;

    if (head->node)
        head->node->pprev = &new->next;
    head->node = new;
}

/**
 * bfdev_hlist_next_add - add a new entry before the one specified.
 * @node: hlist node to add it after, which must be non-NULL.
 * @new: new entry to be added.
 */
static inline void
bfdev_hlist_next_add(struct bfdev_hlist_node *node, struct bfdev_hlist_node *new)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_debug_next_add_check(node, new)))
        return;
#endif

    new->pprev = &node->next;
    new->next = node->next;

    if (new->next)
        new->next->pprev = &node->next;
    node->next = new;
}

/**
 * bfdev_hlist_prev_add - add a new entry before the one specified.
 * @node: hlist node to add it before, which must be non-NULL.
 * @new: new entry to be added.
 */
static inline void
bfdev_hlist_prev_add(struct bfdev_hlist_node *node, struct bfdev_hlist_node *new)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_debug_prev_add_check(node, new)))
        return;
#endif

    new->pprev = node->pprev;
    new->next = node;
    node->pprev = &node->next;
    *node->pprev = new;
}

/**
 * bfdev_hlist_deluf - delete the specified bfdev_hlist_node from its hlist (unsafe).
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_deluf(struct bfdev_hlist_node *node)
{
    struct bfdev_hlist_node **pprev = node->pprev;
    struct bfdev_hlist_node *next = node->next;

    if (next)
        next->pprev = pprev;
    *pprev = next;
}

/**
 * bfdev_hlist_del - delete the specified bfdev_hlist_node from its hlist.
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_del(struct bfdev_hlist_node *node)
{
#ifdef BFDEV_DEBUG_HLIST
    if (bfdev_unlikely(!bfdev_hlist_debug_del_check(node)))
        return;
#endif

    bfdev_hlist_deluf(node);
    node->next = BFDEV_POISON_HLIST1;
    node->pprev = BFDEV_POISON_HLIST2;
}

/**
 * bfdev_hlist_check_empty - check whether the node is head.
 * @head: hlist head to check.
 */
static inline bool
bfdev_hlist_check_empty(const struct bfdev_hlist_head *head)
{
    return !head->node;
}

/**
 * bfdev_hlist_check_first - check whether the node is a header.
 * @head: the head of the hlist.
 * @node: the entry to test.
 */
static inline bool
bfdev_hlist_check_first(const struct bfdev_hlist_head *head,
                        const struct bfdev_hlist_node *node)
{
    return head->node == node;
}

/**
 * bfdev_hlist_check_end - check whether the node is a ending.
 * @node: the entry to test.
 */
static inline bool
bfdev_hlist_check_end(const struct bfdev_hlist_node *node)
{
    return !node->next;
}

/**
 * bfdev_hlist_check_another - check whether has another node.
 * @head: hlist head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_hlist_check_another(const struct bfdev_hlist_head *head,
                          const struct bfdev_hlist_node *node)
{
    return head->node == node && node->next == NULL;
}

/**
 * bfdev_hlist_check_unhashed - check whether the node is reinitialized.
 * @node: hlist node to check.
 */
static inline bool
bfdev_hlist_check_unhashed(const struct bfdev_hlist_node *node)
{
    return !node->pprev;
}

/**
 * bfdev_hlist_head_replace - replace a hlist head with an external head.
 * @old: bfdev_hlist_head for old hlist.
 * @new: bfdev_hlist_head for new hlist.
 */
static inline void
bfdev_hlist_head_replace(struct bfdev_hlist_head *old, struct bfdev_hlist_head *new)
{
    new->node = old->node;
    old->node = NULL;
    if (new->node)
        new->node->pprev = &new->node;
}

/**
 * bfdev_hlist_replace - replace a hlist node with an external node.
 * @old: bfdev_hlist_head for old hlist.
 * @new: bfdev_hlist_head for new hlist.
 */
static inline void
bfdev_hlist_replace(struct bfdev_hlist_node *old, struct bfdev_hlist_node *new)
{
    new->next = old->next;
    new->pprev = old->pprev;
    if (new->next)
        new->next->pprev = &new->next;
    *new->pprev = new;
}

/**
 * bfdev_hlist_del_init -  deletes entry from hlist and reinitialize it.
 * @node: the element to delete from the hlist.
 */
static inline void
bfdev_hlist_del_init(struct bfdev_hlist_node *node)
{
    bfdev_hlist_deluf(node);
    bfdev_hlist_node_init(node);
}

/**
 * bfdev_hlist_entry - get the struct for this entry.
 * @ptr: the &struct bfdev_hlist_node pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the hlist_head within the struct.
 */
#define bfdev_hlist_entry(ptr, type, member) \
    container_of_safe(ptr, type, member)

/**
 * bfdev_hlist_first_entry - get the first element from a hlist.
 * @ptr: the hlist head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the hlist_head within the struct.
 */
#define bfdev_hlist_first_entry(ptr, type, member) \
    bfdev_hlist_entry((ptr)->node, type, member)

/**
 * bfdev_hlist_next_entry - get the next element in hlist.
 * @pos: the type * to cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_next_entry(pos, member) \
    bfdev_hlist_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * bfdev_hlist_for_each - iterate over a hlist.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 * @head: the head for your hlist.
 */
#define bfdev_hlist_for_each(pos, head) \
    for ((pos) = (head)->node; (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_from - iterate over a hlist from the current point.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 */
#define bfdev_hlist_for_each_from(pos) \
    for (; (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_continue - continue iteration over a hlist.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 */
#define bfdev_hlist_for_each_continue(pos) \
    for ((void)((pos) && ((pos) = (pos)->next)); (pos); (pos) = (pos)->next)

/**
 * bfdev_hlist_for_each_safe - iterate over a hlist safe against removal of hlist entry.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 * @head: the head for your hlist.
 */
#define bfdev_hlist_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->node; (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_from_safe - iterate over a hlist safe against removal of hlist entry from the current point.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 */
#define bfdev_hlist_for_each_from_safe(pos, tmp) \
    for (; (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_continue_safe - continue hlist iteration safe against removal.
 * @pos: the &struct bfdev_hlist_node to use as a loop cursor.
 * @tmp: another bfdev_hlist_node to use as temporary storage.
 */
#define bfdev_hlist_for_each_continue_safe(pos, tmp) \
    for ((void)((pos) && ((pos) = (pos)->next)); \
         (pos) && ({(tmp) = (pos)->next; 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = (tmp)->next)))

/**
 * bfdev_hlist_for_each_entry	- iterate over hlist of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your hlist.
 * @member:	the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry(pos, head, member) \
    for ((pos) = bfdev_hlist_first_entry(head, typeof(*(pos)), member); \
         (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_from - continue iteration over hlist of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_from(pos, member) \
    for (; (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_continue - continue iteration over hlist of given type.
 * @pos: the type * to use as a loop cursor.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_continue(pos, member) \
    for ((void)((pos) && ((pos) = bfdev_hlist_next_entry(pos, member))); \
         (pos); (pos) = bfdev_hlist_next_entry(pos, member))

/**
 * bfdev_hlist_for_each_entry_safe - iterate over hlist of given type safe against removal of hlist entry
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage
 * @head: the head for your hlist.
 * @member: the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_hlist_first_entry(head, typeof(*(pos)), member); \
         (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(tmp, member))))

/**
 * bfdev_hlist_for_each_entry_from_safe - iterate over hlist from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member:	the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_from_safe(pos, tmp, member) \
    for (; (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(pos, member))))

/**
 * bfdev_hlist_for_each_entry_continue_safe - continue hlist iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @member:	the name of the bfdev_hlist_node within the struct.
 */
#define bfdev_hlist_for_each_entry_continue_safe(pos, tmp, member) \
    for ((void)((pos) && ((pos) = bfdev_hlist_next_entry(pos, member))); \
         (pos) && ({(tmp) = bfdev_hlist_next_entry(pos, member); 1;}); \
         (pos) = (tmp), ((tmp) && ((tmp) = bfdev_hlist_next_entry(pos, member))))

BFDEV_END_DECLS

#endif /* _BFDEV_HLIST_H_ */
