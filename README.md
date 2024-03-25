# **malloc** implementation with *brk* and *sbrk* syscalls.

* Finding a chunk with the [First Fit Algorithm](https://www.javatpoint.com/first-fit-algorithm-in-c#:~:text=The%20First%20Fit%20Algorithm%20is,the%20required%20amount%20of%20memory.)
* Supports:
```c
void *malloc(size_t size);

void *calloc(size_t number, size_t size);

void free(void *p);

void copy_block(t_block src, t_block dst);

void *realloc(void *p, size_t size);

void *reallocf(void *p, size_t size);
```
* From ["A Malloc Tutorial*"](https://wiki-prog.infoprepa.epita.fr/images/0/04/Malloc_tutorial.pdf) by Marwan Burelle