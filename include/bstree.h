// bstree.h

#pragma once

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bstree;
typedef int_fast32_t (*compare_func_t) (const void* data1, const void* data2);

/*
 * 创建二叉搜索数
 * size: 节点中数据的字节数, 之后的操作都不需要提供该参数了, 所以不支持变长数据
 * compare: 用于比较两个节点数据, 须满足如下返回值的约定:
 *   < 0: 表示 data1 小于 data2
 *   = 0: 表示 data1 等于 data2
 *   > 0: 表示 data1 大于 data2
 * 返回 NULL 表示创建失败, 否则返回创建好的二叉搜索树
 * */
struct bstree* bstree_create (size_t size, compare_func_t compare);

/*
 * 遍历二叉搜索树, 并在每个节点上回调 callback() 函数
 * */
int_fast32_t bstree_walk (struct bstree* bstree, void (*callback) (const void* data));

/*
 * 查找二叉搜索树
 * 通常 data 中包含 key 和其卫星数据, compare() 函数只比较 key. 调用该函数时只需要初始
 * 化 data 中的 key 即可. 如果搜索到匹配数据, 则复制到 data 中, 并返回 0, 否则返回 -1.
 * */
int_fast32_t bstree_search (struct bstree* bstree, void* data);

/*
 * 最小元素
 * 如果二叉搜索树为空或参数错误, 则返回 -1, 否则返回 0, 并且把最小数据复制到 data 中
 * */
int_fast32_t bstree_min (struct bstree* bstree, void* data);

/*
 * 最大元素
 * 如果二叉搜索树为空或参数错误, 则返回 -1, 否则返回 0, 并且把最大数据复制到 data 中
 * */
int_fast32_t bstree_max (struct bstree* bstree, void* data);

/*
 * 插入二叉树节点
 * */
int_fast32_t bstree_insert (struct bstree* bstree, const void* data);

/*
 * 删除二叉树节点
 * */
int_fast32_t bstree_delete (struct bstree* bstree, void* data);

/*
 * 销毁二叉树
 * */
void bstree_destroy (struct bstree* bstree);

#ifdef __cplusplus
}
#endif

