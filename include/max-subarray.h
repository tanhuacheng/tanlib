// max-subarray.h

#pragma once

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t start;
    size_t end;
    int sum;
} max_subarray_result_int_t;

/*
 * 查找 array(int 类型的数组) 中从 start(包含) 到 end(不包含) 范围内的最大子数组
 * 返回 0 表示执行成功且结果写入 result 中, 否则返回 -1(array 为空或 start >= end)
 * */
int max_subarray_int (const int* array, size_t start, size_t end, max_subarray_result_int_t* result);
int max_subarray_int_r (const int* array, size_t start, size_t end, max_subarray_result_int_t* result);

#ifdef __cplusplus
}
#endif

// end of file
