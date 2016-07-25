// foreach.h

#pragma once

#ifdef foreach
#error Macro "foreach" is already defined in other file(s)
#endif

/*
 * 1. 目前只能在 gcc 下使用, 且需要包含选项 --std gnu99 或 --std gnu11
 * 2. varp 是一个遍历 array 的指针
 * 3. array 只能是数组(包括结构体数组和一般类型数组)
 *
 *  int array[] = {1, 2, 3, 4};
 *  foreach (x, array) {
 *      printf("%d\n", *x);
 *  }
 *
 * */
#define foreach(varp, array) \
    for (typeof(array[0]) *varp = array; varp < (array + (sizeof(array) / sizeof(array[0]))); ++varp)

