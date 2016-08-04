// sort.h

#pragma once

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 插入排序.
 * 使用 compare 定义的行为, 对从 base 开始的 nmemb 个大小为 size 的数据进行由小到大的排序.
 * 返回 0 表示执行成功; -1 表示参数错误.
 * */
int insert_sort (void* base, size_t nmemb, size_t size, compare_t compare);

/*
 * 归并排序.
 * 使用 compare 定义的行为, 对从 base 开始的 nmemb 个大小为 size 的数据进行由小到大的排序.
 * 返回 0 表示执行成功; -1 表示参数错误.
 * */
int merge_sort (void* base, size_t nmemb, size_t size, compare_t compare);

#ifdef __cplusplus
}
#endif

// end of file
