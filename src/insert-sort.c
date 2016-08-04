// insert_sort.c

#include <stdlib.h>
#include <string.h>
#include "sort.h"

int insert_sort (void* base, size_t nmemb, size_t size, compare_t compare)
{
    if ((NULL == base) || (NULL == compare)) {
        return -1;
    }
    if (nmemb <= 1) {
        return 0;
    }
    if (0 == size) {
        return -1;
    }

    uint8_t key[size];

    for (size_t i = 1; i < nmemb; i++) {
        ssize_t j = i - 1;
        memcpy(key, INDEXOF(base, i, size), size);
        while ((j >= 0) && (compare(key, INDEXOF(base, j, size)) < 0)) {
            memcpy(INDEXOF(base, j + 1, size), INDEXOF(base, j, size), size);
            j--;
        }
        memcpy(INDEXOF(base, j + 1, size), key, size);
    }

    return 0;
}

// end of file
