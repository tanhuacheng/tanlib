#include <stdio.h>
#include "foreach.h"
#include "max-subarray.h"

int main (int argc, char* argv[])
{
    static const int array[] = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    int length = sizeof(array)/sizeof(array[0]);

    printf("array:\n");
    foreach (x, array) {
        printf("%d ", *x);
    }
    printf("\n\n");

    max_subarray_result_int_t result;
    max_subarray_int(array, 0, length, &result);
    printf("result: start: %ld, end: %ld, sum: %d\n", result.start, result.end, result.sum);
    for (size_t i = result.start; i < result.end; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
