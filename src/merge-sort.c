// merge_sort.c

#include <stdlib.h>
#include <string.h>
#include "sort.h"

struct aux_data {
    void* base;
    void* temp;
    size_t size;
    compare_t compare;
};

static void merge (struct aux_data* aux, size_t start, size_t middle, size_t end)
{
    size_t left = start;
    size_t right = middle;
    size_t index = 0;

    while ((left < middle) && (right < end)) {
        if (aux->compare(INDEXOF(aux->base, left, aux->size), INDEXOF(aux->base, right, aux->size)) < 0) {
            memcpy(INDEXOF(aux->temp, index, aux->size), INDEXOF(aux->base, left, aux->size), aux->size);
            left++;
        } else {
            memcpy(INDEXOF(aux->temp, index, aux->size), INDEXOF(aux->base, right, aux->size), aux->size);
            right++;
        }
        index++;
    }
    if (left < middle) {
        memcpy(INDEXOF(aux->temp, index, aux->size), INDEXOF(aux->base, left, aux->size), (middle - left) * aux->size);
    } else if (right < end) {
        memcpy(INDEXOF(aux->temp, index, aux->size), INDEXOF(aux->base, right, aux->size), (end - right) * aux->size);
    }
    memcpy(INDEXOF(aux->base, start, aux->size), aux->temp, (end - start) * aux->size);
}

static void __merge_sort (struct aux_data* aux, size_t start, size_t end)
{
    if (start < (end - 1)) {
        size_t middle = (start + end) >> 1;
        __merge_sort(aux, start, middle);
        __merge_sort(aux, middle, end);
        merge(aux, start, middle, end);
    }
}

int merge_sort (void* base, size_t nmemb, size_t size, compare_t compare)
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

    void* temp = calloc(nmemb, size);
    if (NULL == temp) {
        return -1;
    }
    __merge_sort(&(struct aux_data){base: base, temp: temp, size: size, compare: compare}, 0, nmemb);
    free(temp);

    return 0;
}

// end of file
