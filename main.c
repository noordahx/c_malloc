#include <stdio.h>

#include "malloc.h"

int main() {
    printf("Hello, World!\n");
    int *a = malloc(sizeof(int) * 2);
    a[0] = 1;
    a[1] = 2;
    printf("%d %d\n", a[0], a[1]);
    free(a);
    printf("%d %d\n", a[0], a[1]);
    return 0;
}
