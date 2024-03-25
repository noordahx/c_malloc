#include <stdio.h>

#include "malloc.h"

int main() {
    printf("Hello, World!\n");
    size_t *a = malloc(sizeof(size_t) * 2);
    a[0] = 1;
    a[1] = 2;
    printf("%ld %ld\n", a[0], a[1]);
    free(a);

    return 0;
}
