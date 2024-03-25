//
// Created by ntaumergenov on 3/25/2024.
//

#include "malloc.h"

#include <sys/types.h>
#include <unistd.h>

void *base = NULL;

t_block find_block(t_block *last, size_t size) {
    t_block b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b -> next;
    }
    return (b);
}

t_block extend_heap(t_block last, size_t s) {
    int             sb;
    t_block         b;
    b = sbrk(0);
    sb = (int)sbrk(BLOCK_SIZE + s);
    if (sb < 0)
        return (NULL);
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;
    if (last)
        last->next = b;
    b->free = 0;
    return (b);
}

void split_block(t_block b, size_t s) {
    t_block         new;
    new = (t_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    new->ptr = new->data;
    b->size = s;
    b->next = new;
    if (new->next)
        new->next->prev = new;
}

void *malloc(size_t size) {
    t_block         b, last;
    size_t          s;
    s = align4(size);
    if (base) {
        /* First find a block */
        last = base;
        b = find_block(&last, s);
        if (b) {
            /* can we split */
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);
            b->free = 0;
        } else {
            /* No fitting block, extend the heap */
            b = extend_heap(last, s);
            if (!b)
                return (NULL);
        }
    } else {
        /* first time */
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->data);
}

void *calloc(size_t number, size_t size) {
    size_t      *new_block;
    size_t      s4, i;
    new_block = malloc(number * size);
    if (new_block) {
        s4 = align4(number * size) << 2;
        for (i = 0; i<s4; i++)
            new_block[i] = 0;
    }
    return (new_block);
}

t_block fusion(t_block b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->next = b;
    }
    return (b);
}

t_block get_block(void *p) {
    char *tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

int valid_addr(void *p) {
    if (base) {
        if (p > base && p < sbrk(0)) {
            return (p == (get_block(p))->ptr);
        }
    }
    return (0);
}

void free(void *p) {
    t_block b;
    if (valid_addr(p)) {
        b = get_block(p);
        b->free = 1;
        /* fusion with previous if possible */
        if (b->prev && b->prev->free)
            b = fusion(b->prev);
        if (b->next)
            fusion(b);
        else {
            if (b->prev)
                b->prev->next = NULL;
            else
                /* No more block */
                base = NULL;
            brk(b);
        }
    }
}

void copy_block(t_block src, t_block dst) {
    int             *sdata, *ddata;
    size_t          i;
    sdata = src->ptr;
    ddata = dst->ptr;
    for (i=0; i*4 < src->size && i*4 < dst->size; i++)
        ddata[i] = sdata[i];
}

void *realloc(void *p, size_t size) {
    size_t          s;
    t_block         b, new;
    void            *newp;
    if (!p)
        return (malloc(size));
    if (valid_addr(p)) {
        s = align4(size);
        b = get_block(p);
        if (b->size >= s) {
            if (b->size - s >= (BLOCK_SIZE + 4))
                split_block(b, s);
        }
        else {
            /* Try fusion with next if possible */
            if (b->next && b->next->free
                && (b->size + BLOCK_SIZE + b->next->size) >= s) {
                fusion(b);
                if (b->size -s >= (BLOCK_SIZE + 4))
                    split_block(b, s);
            }
            else {
                /* realloc with a new block */
                newp = malloc(s);
                if (!newp)
                    return (NULL);
                new = get_block(newp);
                copy_block(b, new);
                free(p);
                return (newp);
            }
        }
        return (p);
    }
    return (NULL);
}

void *reallocf(void *p, size_t size) {
    void *newp;
    newp = realloc(p, size);
    if (!newp)
        free(p);
    return (newp);
}
