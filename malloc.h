//
// Created by ntaumergenov on 3/25/2024.
//
#include <stdio.h>

#ifndef C_MALLOC_MALLOC_H
#define C_MALLOC_MALLOC_H



// arithmetic trick to get nearest 4 byte chunk size
#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

// size of meta-data block
#define BLOCK_SIZE 20

extern void *base;

typedef struct s_block *t_block;

struct s_block {
    size_t          size;
    struct s_block  *next;
    struct s_block  *prev;
    int             free;
    void            *ptr;
    char            data[1];
};

t_block find_block(t_block *last, size_t size);

t_block extend_heap(t_block last, size_t s);

void split_block(t_block b, size_t);

t_block fusion(t_block b);

t_block get_block(void *p);

int valid_addr(void *p);

void *malloc(size_t size);

void *calloc(size_t number, size_t size);

void free(void *p);

void copy_block(t_block src, t_block dst);

void *realloc(void *p, size_t size);

void *reallocf(void *p, size_t size);

#endif //C_MALLOC_MALLOC_H
