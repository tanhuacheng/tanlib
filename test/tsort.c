// tsort.c

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "foreach.h"
#include "sort.h"

#define MEMBOF(array) (sizeof(array) / sizeof(array[0]))

static int compare (const void* data1, const void* data2)
{
    if (*(int*)data1 < *(int*)data2) {
        return -1;
    } else if (*(int*)data1 == *(int*)data2) {
        return 0;
    } else {
        return 1;
    }
}

static void test_insert_sort (void)
{
    const static int array_sorted[] = {0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
    static int array[] = {1, 8, 4, 6, 2, 9, 3, 0, 5, 7, 5};
    static int temp[MEMBOF(array)];

    assert(MEMBOF(array_sorted) == MEMBOF(array));

    assert(insert_sort(NULL, 0, 0, NULL) < 0);

    memcpy(temp, array, sizeof(array));
    assert(insert_sort(temp, 1, sizeof(temp[0]), compare) == 0);
    assert(temp[0] == array[0]);

    memcpy(temp, array, sizeof(array));
    assert(insert_sort(temp, 2, sizeof(temp[0]), compare) == 0);
    assert(temp[0] == ((compare(&array[0], &array[1]) < 0) ? array[0] : array[1]));

    assert(insert_sort(array, MEMBOF(array), sizeof(array[0]), compare) == 0);
    assert(0 == memcmp(array, array_sorted, sizeof(array)));

    printf("%s ... pass\n", __func__);
}

static void test_merge_sort (void)
{
    const static int array_sorted[] = {0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
    static int array[] = {1, 8, 4, 6, 2, 9, 3, 0, 5, 7, 5};
    static int temp[MEMBOF(array)];

    assert(MEMBOF(array_sorted) == MEMBOF(array));

    assert(merge_sort(NULL, 0, 0, NULL) < 0);

    memcpy(temp, array, sizeof(array));
    assert(merge_sort(temp, 1, sizeof(temp[0]), compare) == 0);
    assert(temp[0] == array[0]);

    memcpy(temp, array, sizeof(array));
    assert(merge_sort(temp, 2, sizeof(temp[0]), compare) == 0);
    assert(temp[0] == ((compare(&array[0], &array[1]) < 0) ? array[0] : array[1]));

    assert(merge_sort(array, MEMBOF(array), sizeof(array[0]), compare) == 0);
    assert(0 == memcmp(array, array_sorted, sizeof(array)));

    printf("%s ... pass\n", __func__);
}

static uint64_t time_diff (struct timeval* b, struct timeval* e)
{
    return (e->tv_sec - b->tv_sec) * 1000 * 1000 + e->tv_usec - b->tv_usec;
}

static void test_time_insert_vs_merge_vs_qsort (void)
{
#define LENGTH_LESS 30
#define LENGTH_LARGE (1024 * 16)
    static int array[LENGTH_LARGE];
    static int temp[LENGTH_LARGE];

    srand(time(NULL));
    foreach (x, array) { *x = rand(); }

    struct timeval tv_b, tv_e;

    // less
    memcpy(temp, array, LENGTH_LESS * sizeof(array[0]));
    gettimeofday(&tv_b, NULL);
    insert_sort(temp, LENGTH_LESS, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("less: time of insert_sort = %ld\n", time_diff(&tv_b, &tv_e));

    memcpy(temp, array, LENGTH_LESS * sizeof(array[0]));
    gettimeofday(&tv_b, NULL);
    merge_sort(temp, LENGTH_LESS, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("less: time of merge_sort = %ld\n", time_diff(&tv_b, &tv_e));

    // large
    memcpy(temp, array, LENGTH_LARGE * sizeof(array[0]));
    gettimeofday(&tv_b, NULL);
    insert_sort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of insert_sort = %ld\n", time_diff(&tv_b, &tv_e));

    gettimeofday(&tv_b, NULL);
    insert_sort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of insert_sort(sorted) = %ld\n", time_diff(&tv_b, &tv_e));

    memcpy(temp, array,  LENGTH_LARGE* sizeof(array[0]));
    gettimeofday(&tv_b, NULL);
    merge_sort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of merge_sort = %ld\n", time_diff(&tv_b, &tv_e));

    gettimeofday(&tv_b, NULL);
    merge_sort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of merge_sort(sorted) = %ld\n", time_diff(&tv_b, &tv_e));

    memcpy(temp, array,  LENGTH_LARGE* sizeof(array[0]));
    gettimeofday(&tv_b, NULL);
    qsort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of qsort = %ld\n", time_diff(&tv_b, &tv_e));

    gettimeofday(&tv_b, NULL);
    qsort(temp, LENGTH_LARGE, sizeof(array[0]), compare);
    gettimeofday(&tv_e, NULL);
    printf("large: time of qsort(sorted) = %ld\n", time_diff(&tv_b, &tv_e));
}

int main (int argc, char* argv[])
{
    test_insert_sort();
    test_merge_sort();
    test_time_insert_vs_merge_vs_qsort();

    return 0;
}

// end of file
