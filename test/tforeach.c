// tforeach.c

#include <stdio.h>
#include "foreach.h"

int main (int argc, char* argv[])
{
    int array[] = {1, 2, 3, 4, 5};

    foreach (x, array) {
        printf("%d\n", *x);
        (*x)++;
    }

    printf("\n");
    foreach (x, array) {
        printf("%d\n", *x);
    }

    struct {
        int x;
        int y;
    } points[] = {[0] = {x: 1, y: 2}, [2] = {y: 20, x: 10}};

    printf("\n");
    foreach (p, points) {
        printf("x: %d, y: %d\n", p->x, p->y);
    }
}

