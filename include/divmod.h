/*
*********************************************************************************************************
* 文件: divmod.h
* 版本: V0.02
* 创建: Thu May 26 15:58:31 2016
* 作者: 谭化成
* 描述: 除法和取余优化
*
* 版本历史
* +----------------------------------------------------------------------------------------------------+
* |    时间    |  版本  |                              描 述                              |    作者    |
* +----------------------------------------------------------------------------------------------------+
* | 2016/05/26 |  0.01  | 初版完成                                                        |  谭化成    |
* +----------------------------------------------------------------------------------------------------+
* | 2016/05/31 |  0.02  | 更改文件名为divmod.h， 添加函数 imod                            |  谭化成    |
* +----------------------------------------------------------------------------------------------------+
*
*********************************************************************************************************
*/

#ifndef DIVMOD_H_
#define DIVMOD_H_

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
  extern "C" {
#endif

/* 宏定义 ---------------------------------------------------------------------------------------------*/

/* 数据类型声明 ---------------------------------------------------------------------------------------*/

/* 函数声明 -------------------------------------------------------------------------------------------*/
// inc加1后对mod取余, 要求 inc < mod
static inline uint32_t incmod (uint32_t inc, const uint32_t mod)
{
    if ((++inc) >= mod) {
        inc -= mod;
    }

    return inc;
}

// 计算x/y: z在原值上加商, 返余数. 注意, 仅在x/y较小时用
static inline uint32_t imod (uint32_t x, uint32_t y, uint32_t* z)
{
    while (x >= y) {
        x -= y;
        *z = *z + 1;
    }

    return x;
}

#ifdef __cplusplus
  }
#endif

#endif /* DIVMOD_H_ */
/********************************************* end of file *********************************************/
