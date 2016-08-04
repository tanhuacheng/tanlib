// typedefs.h

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 获取由 base 开始的第 index 个大小为 size 的内存单元地址(每个内存单元的大小都是 size)
 * */
#define INDEXOF(base, index, size) ((void*)(base) + (index) * (size))

/*
 * 通用比较函数.
 * 返回小于 0 表示第一个数据小于第二个; 等于 0 表示两者相等; 否则表示第一个数据大于第二个.
 * */
typedef int (*compare_t) (const void*, const void*);

#ifdef __cplusplus
}
#endif

// end of file
