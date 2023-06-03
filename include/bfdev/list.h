/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#ifndef _BFDEV_LIST_H_
#define _BFDEV_LIST_H_

#include <bfdev/config.h>
#include <bfdev/cdefs.h>
#include <bfdev/stddef.h>
#include <bfdev/stdbool.h>
#include <bfdev/compiler.h>
#include <bfdev/container.h>
#include <bfdev/poison.h>

BFDEV_BEGIN_DECLS

struct bfdev_list_head {
    struct bfdev_list_head *prev;
    struct bfdev_list_head *next;
};

#define BFDEV_LIST_HEAD_STATIC(name) \
    {&(name), &(name)}

#define BFDEV_LIST_HEAD_INIT(name) \
    (struct bfdev_list_head) BFDEV_LIST_HEAD_STATIC(name)

#define BFDEV_LIST_HEAD(name) \
    struct bfdev_list_head name = BFDEV_LIST_HEAD_INIT(name)

typedef long (*bfdev_list_cmp_t)(const struct bfdev_list_head *, const struct bfdev_list_head *, void *);
extern void bfdev_list_qsort(struct bfdev_list_head *head, bfdev_list_cmp_t cmp, void *data);
extern void bfdev_list_bsort(struct bfdev_list_head *head, bfdev_list_cmp_t cmp, void *data);

#ifdef BFDEV_DEBUG_LIST
extern bool bfdev_list_debug_add_check(struct bfdev_list_head *prev, struct bfdev_list_head *next, struct bfdev_list_head *new);
extern bool bfdev_list_debug_del_check(struct bfdev_list_head *node);
#endif

static inline void
bfdev_list_insert(struct bfdev_list_head *prev, struct bfdev_list_head *next,
                  struct bfdev_list_head *new)
{
#ifdef BFDEV_DEBUG_LIST
    if (unlikely(!bfdev_list_debug_add_check(prev, next, new)))
        return;
#endif

    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

/**
 * bfdev_list_head_init - initialize a list head structure.
 * @head: list head structure to be initialized.
 */
static inline void
bfdev_list_head_init(struct bfdev_list_head *head)
{
    head->prev = head;
    head->next = head;
}

/**
 * bfdev_list_add - add a new node next old node.
 * @node: list head to add it next.
 * @new: new entry to be added.
 */
static inline void
bfdev_list_add(struct bfdev_list_head *node, struct bfdev_list_head *new)
{
    bfdev_list_insert(node, node->next, new);
}

/**
 * bfdev_list_add_prev - add a new node after old node.
 * @node: list head to add it prev.
 * @new: new entry to be added.
 */
static inline void
bfdev_list_add_prev(struct bfdev_list_head *node, struct bfdev_list_head *new)
{
    bfdev_list_insert(node->prev, node, new);
}

/**
 * bfdev_list_deluf - deletes entry from list (unsafe).
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_deluf(struct bfdev_list_head *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/**
 * bfdev_list_del - deletes entry from list.
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_del(struct bfdev_list_head *node)
{
#ifdef BFDEV_DEBUG_LIST
    if (unlikely(!bfdev_list_debug_del_check(node)))
        return;
#endif

    bfdev_list_deluf(node);
    node->next = BFDEV_POISON_LIST1;
    node->prev = BFDEV_POISON_LIST2;
}

/**
 * bfdev_list_check_empty - check whether a list is empty.
 * @head: list head to check.
 */
static inline bool
bfdev_list_check_empty(const struct bfdev_list_head *head)
{
    return head->next == head;
}

/**
 * bfdev_list_check_head - check whether the node is head.
 * @head: the head of the list
 * @list: the entry to test
 */
static inline bool
bfdev_list_check_head(const struct bfdev_list_head *head,
                      const struct bfdev_list_head *node)
{
    return head == node;
}

/**
 * bfdev_list_check_first - check whether the node is a header.
 * @head: the head of the list.
 * @node: the entry to test.
 */
static inline bool
bfdev_list_check_first(const struct bfdev_list_head *head,
                       const struct bfdev_list_head *node)
{
    return node->prev == head;
}

/**
 * bfdev_list_check_end - check whether the node is a ending.
 * @head: the head of the list.
 * @node: the entry to test.
 */
static inline bool
bfdev_list_check_end(const struct bfdev_list_head *head,
                     const struct bfdev_list_head *node)
{
    return node->next == head;
}

/**
 * bfdev_list_check_another - check whether has another node.
 * @head: list head to check.
 * @node: the unique node.
 */
static inline bool
bfdev_list_check_another(const struct bfdev_list_head *head,
                         const struct bfdev_list_head *node)
{
    return head->next == node && head->prev == node;
}

/**
 * bfdev_list_check_outsize - check whether the node is outside the list.
 * @node: list entry to check.
 */
static inline bool
bfdev_list_check_outsize(const struct bfdev_list_head *node)
{
    return !node->next || node->next == BFDEV_POISON_LIST1;
}

/**
 * bfdev_list_replace - replace a list node with an external node.
 * @old: the element to be replaced.
 * @new: the new element to insert.
 */
static inline void
bfdev_list_replace(struct bfdev_list_head *old, struct bfdev_list_head *new)
{
    new->prev = old->prev;
    new->next = old->next;
    new->prev->next = new;
    new->next->prev = new;
}

static inline void
bfdev_list_relocate(struct bfdev_list_head *prev, struct bfdev_list_head *next,
                    struct bfdev_list_head *list)
{
    struct bfdev_list_head *first = list->next;
    struct bfdev_list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 * bfdev_list_splice - join two lists, this is designed for stacks.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice(struct bfdev_list_head *head, struct bfdev_list_head *list)
{
    if (!bfdev_list_check_empty(list))
        bfdev_list_relocate(head, head->next, list);
}

/**
 * bfdev_list_splice_prev - join two lists, each list being a queue.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_prev(struct bfdev_list_head *head, struct bfdev_list_head *list)
{
    if (!bfdev_list_check_empty(list))
        bfdev_list_relocate(head->prev, head, list);
}

/**
 * bfdev_list_move - move the node to the next of the node.
 * @head: the head that will precede our entry.
 * @node: the entry to move.
 */
static inline void
bfdev_list_move(struct bfdev_list_head *head, struct bfdev_list_head *node)
{
    bfdev_list_del(node);
    bfdev_list_add(head, node);
}

/**
 * bfdev_list_move_prev - move the node to the prev of the node.
 * @head: the head that will follow our entry.
 * @node: the entry to move.
 */
static inline void
bfdev_list_move_prev(struct bfdev_list_head *head, struct bfdev_list_head *node)
{
    bfdev_list_del(node);
    bfdev_list_add_prev(head, node);
}

/**
 * bfdev_list_swap - replace entry1 with entry2 and re-add entry1 at entry2's position.
 * @node1: the location to place entry2.
 * @node2: the location to place entry1.
 */
static inline void
bfdev_list_swap(struct bfdev_list_head *node1, struct bfdev_list_head *node2)
{
    struct bfdev_list_head *prev = node2->prev;

    bfdev_list_del(node2);
    bfdev_list_replace(node1, node2);

    if (prev == node1)
        prev = node2;
    bfdev_list_add(prev, node1);
}

/**
 * bfdev_list_del_init - deletes entry from list and reinitialize it.
 * @node: the element to delete from the list.
 */
static inline void
bfdev_list_del_init(struct bfdev_list_head *node)
{
    bfdev_list_del(node);
    bfdev_list_head_init(node);
}

/**
 * bfdev_list_replace_init - replace old entry by new one and initialize the old one.
 * @old: the element to be replaced.
 * @new: the new element to insert.
 */
static inline void
bfdev_list_replace_init(struct bfdev_list_head *old, struct bfdev_list_head *new)
{
    bfdev_list_replace(old, new);
    bfdev_list_head_init(old);
}

/**
 * bfdev_list_splice_init - join two lists and reinitialise the emptied list.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_init(struct bfdev_list_head *head, struct bfdev_list_head *list)
{
    if (!bfdev_list_check_empty(list)) {
        bfdev_list_splice(head, list);
        bfdev_list_head_init(list);
    }
}

/**
 * bfdev_list_splice_tail_init - join two lists and reinitialise the emptied list.
 * @head: the place to add it in the first list.
 * @list: the new list to add.
 */
static inline void
bfdev_list_splice_tail_init(struct bfdev_list_head *head, struct bfdev_list_head *list)
{
    if (!bfdev_list_check_empty(list)) {
        bfdev_list_splice_prev(head, list);
        bfdev_list_head_init(list);
    }
}

/**
 * bfdev_list_entry - get the struct for this entry.
 * @ptr: the &struct list head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * bfdev_list_entry_check_head - test if the entry points to the head of the list.
 * @pos: the type * to cursor
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_entry_check_head(pos, head, member) \
    (&(pos)->member == (head))

/**
 * bfdev_list_first_entry - get the first element from a list.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_first_entry(ptr, type, member) \
    bfdev_list_entry((ptr)->next, type, member)

/**
 * bfdev_list_last_entry - get the last element from a list.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_last_entry(ptr, type, member) \
    bfdev_list_entry((ptr)->prev, type, member)

/**
 * bfdev_list_next_entry - get the next element in list.
 * @pos: the type * to cursor.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_next_entry(pos, member) \
    bfdev_list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * bfdev_list_prev_entry - get the prev element in list.
 * @pos: the type * to cursor.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_prev_entry(pos, member) \
    bfdev_list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 * bfdev_list_first_entry_or_null - get the first element from a list or null.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_first_entry_or_null(ptr, type, member) ({ \
    struct bfdev_list_head *head__ = (ptr); \
    struct bfdev_list_head *pos__ = head__->next; \
    pos__ != head__ ? bfdev_list_entry(pos__, type, member) : NULL; \
})

/**
 * bfdev_list_last_entry_or_null - get the last element from a list or null.
 * @ptr: the list head to take the element from.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_last_entry_or_null(ptr, type, member) ({ \
    struct bfdev_list_head *head__ = (ptr); \
    struct bfdev_list_head *pos__ = head__->prev; \
    pos__ != head__ ? bfdev_list_entry(pos__, type, member) : NULL; \
})

/**
 * bfdev_list_next_entry_or_null - get the next element in list or null.
 * @pos: the type * to cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_next_entry_or_null(pos, head, member) ({ \
    typeof(*(pos)) *pos__; \
    pos__ = bfdev_list_entry((pos)->member.next, typeof(*(pos)), member); \
    bfdev_list_entry_check_head(pos__, head, member) ? NULL : pos__; \
})

/**
 * bfdev_list_prev_entry_or_null - get the prev element in list or null.
 * @pos: the type * to cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_prev_entry_or_null(pos, head, member) ({ \
    typeof(*(pos)) *pos__; \
    pos__ = bfdev_list_entry((pos)->member.prev, typeof(*(pos)), member); \
    bfdev_list_entry_check_head(pos__, head, member) ? NULL : pos__; \
})

/**
 * bfdev_list_for_each - iterate over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each(pos, head) \
    for ((pos) = (head)->next; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse - iterate over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse(pos, head) \
    for ((pos) = (head)->prev; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_from - iterate over a list from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_from(pos, head) \
    for (; !bfdev_list_check_head(head, pos); (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_from - iterate over a list backwards from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_from(pos, head) \
    for (; !bfdev_list_check_head(head, pos); (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_continue - continue iteration over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue(pos, head) \
    for ((pos) = (pos)->next; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_continue - continue iteration over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue(pos, head) \
    for ((pos) = (pos)->prev; !bfdev_list_check_head(head, pos); \
         (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_continue_check - check head and continue iteration over a list.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_check(pos, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && ((pos) = (pos)->next)); \
         !bfdev_list_check_head(head, pos); (pos) = (pos)->next)

/**
 * bfdev_list_for_each_reverse_continue_check - check head and continue iteration over a list backwards.
 * @pos: the &struct list head to use as a loop cursor.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_check(pos, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && ((pos) = (pos)->prev)); \
         !bfdev_list_check_head(head, pos); (pos) = (pos)->prev)

/**
 * bfdev_list_for_each_safe - iterate over a list safe against removal of list entry.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_safe(pos, tmp, head) \
    for ((pos) = (head)->next, (tmp) = (pos)->next; \
         !bfdev_list_check_head((pos), head); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_safe(pos, tmp, head) \
    for ((pos) = (head)->prev, (tmp) = (pos)->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_from_safe - iterate over a list safe against removal of list entry from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_from_safe(pos, tmp, head) \
    for ((tmp) = (pos)->next; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_from_safe - iterate backwards over list safe against removal from the current point.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_from_safe(pos, tmp, head) \
    for ((tmp) = pos->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_continue_safe - continue list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_safe(pos, tmp, head) \
    for ((pos) = (pos)->next, (tmp) = (pos)->next; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_safe(pos, tmp, head) \
    for ((pos) = pos->prev, (tmp) = pos->prev; \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_continue_check_safe - check head and continue list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_continue_check_safe(pos, tmp, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && \
         ((pos) = (pos)->next, (tmp) = (pos)->next)); \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->next)

/**
 * bfdev_list_for_each_reverse_continue_check_safe - check head and continue backwards over list iteration safe against removal.
 * @pos: the &struct list head to use as a loop cursor.
 * @tmp: another list head to use as temporary storage.
 * @head: the head for your list.
 */
#define bfdev_list_for_each_reverse_continue_check_safe(pos, tmp, head) \
    for ((void)(!bfdev_list_check_head(head, pos) && \
         ((pos) = pos->prev, (tmp) = pos->prev)); \
         !bfdev_list_check_head(head, pos); \
         (pos) = (tmp), (tmp) = (tmp)->prev)

/**
 * bfdev_list_for_each_entry - iterate over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry(pos, head, member) \
    for ((pos) = bfdev_list_first_entry(head, typeof(*(pos)), member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse(pos, head, member) \
    for ((pos) = bfdev_list_last_entry(head, typeof(*(pos)), member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_from - iterate over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_from(pos, head, member) \
    for (; !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_from - iterate backwards over list of given type from the current point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_from(pos, head, member) \
    for (; !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_continue - continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue(pos, head, member) \
    for ((pos) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_continue - iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue(pos, head, member) \
    for ((pos) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_continue_check - check head and continue iteration over list of given type.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_check(pos, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_next_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_next_entry(pos, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_check - check head and iterate backwards from the given point.
 * @pos: the type * to use as a loop cursor.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_check(pos, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_prev_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = bfdev_list_prev_entry(pos, member))

/**
 * bfdev_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member: the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_first_entry(head, typeof(*(pos)), member), \
         (tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_safe - iterate backwards over list safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_last_entry(head, typeof(*pos), member), \
         (tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_from_safe - iterate over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_from_safe(pos, tmp, head, member) \
    for ((tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_from_safe - iterate backwards over list from current point safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_from_safe(pos, tmp, head, member) \
    for ((tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_continue_safe - continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_next_entry(pos, member), \
         (tmp) = bfdev_list_next_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_safe - continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_safe(pos, tmp, head, member) \
    for ((pos) = bfdev_list_prev_entry(pos, member), \
         (tmp) = bfdev_list_prev_entry(pos, member); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_continue_check_safe - check head and continue list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_continue_check_safe(pos, tmp, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_next_entry(pos, member), \
         (tmp) = bfdev_list_next_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_next_entry(tmp, member))

/**
 * bfdev_list_for_each_entry_reverse_continue_check_safe - check head and continue backwards over list iteration safe against removal.
 * @pos: the type * to use as a loop cursor.
 * @tmp: another type * to use as temporary storage.
 * @head: the head for your list.
 * @member:	the name of the list head within the struct.
 */
#define bfdev_list_for_each_entry_reverse_continue_check_safe(pos, tmp, head, member) \
    for ((void)(!bfdev_list_entry_check_head(pos, head, member) && \
         ((pos) = bfdev_list_prev_entry(pos, member), \
         (tmp) = bfdev_list_prev_entry(pos, member))); \
         !bfdev_list_entry_check_head(pos, head, member); \
         (pos) = (tmp), (tmp) = bfdev_list_prev_entry(tmp, member))

BFDEV_END_DECLS

#endif /* _BFDEV_LIST_H_ */
