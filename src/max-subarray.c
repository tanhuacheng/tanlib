// max-subarray.c

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "max-subarray.h"

static void max_subarray_int_cross (const int* array, size_t start, size_t mid, size_t end, max_subarray_result_int_t* result)
{
    assert((start < mid) && (mid < end));

    int left_sum = INT_MIN;
    int sum = 0;

    for (ssize_t i = mid - 1; i >= (ssize_t)start; i--) {
        sum += array[i];
        if (sum > left_sum) {
            left_sum = sum;
            result->start = i;
        }
    }

    int rigth_sum = INT_MIN;
    sum = 0;

    for (size_t i = mid; i < end; i++) {
        sum += array[i];
        if (sum > rigth_sum) {
            rigth_sum = sum;
            result->end = i;
        }
    }
    result->end++;
    result->sum = left_sum + rigth_sum;
}

int max_subarray_int (const int* array, size_t start, size_t end, max_subarray_result_int_t* result)
{
    if ((NULL == array) || (start >= end)) {
        return -1;
    }

    if (start == (end - 1)) {
        result->start = start;
        result->end = end;
        result->sum = array[start];
    } else {
        max_subarray_result_int_t left;
        max_subarray_result_int_t rigth;
        max_subarray_result_int_t cross;
        size_t mid = (start + end) >> 1;

        max_subarray_int(array, start, mid, &left);
        max_subarray_int(array, mid, end, &rigth);
        max_subarray_int_cross(array, start, mid, end, &cross);

        if ((left.sum >= rigth.sum) && (left.sum >= cross.sum)) {
            *result = left;
        } else if (cross.sum >= rigth.sum) {
            *result = cross;
        } else {
            *result = rigth;
        }
    }

    return 0;
}

int max_subarray_int_r (const int* array, size_t start, size_t end, max_subarray_result_int_t* result)
{
    if ((NULL == array) || (start >= end)) {
        return -1;
    }

    result->start = start;
    result->end = start;
    result->sum = array[start];

    size_t cur_start = start;
    int cur_sum = array[start];

    for (size_t i = start + 1; i < end; i++) {
        if (cur_sum < 0) {
            cur_start = i;
            cur_sum = array[i];
        } else {
            cur_sum += array[i];
        }
        if (cur_sum > result->sum) {
            result->start = cur_start;
            result->end = i;
            result->sum = cur_sum;
        }
    }
    result->end++;

    return 0;
}

// end of file
