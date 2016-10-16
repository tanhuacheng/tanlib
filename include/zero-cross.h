/*
 * =====================================================================================
 *
 *       Filename:  zero-cross.h
 *
 *    Description:  计算信号的短时过零数
 *
 *        Version:  1.0
 *        Created:  2016年10月16日 17:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  tanhuacheng (tanhc), tanhc.gz@gmail.com
 *   Organization:  tanlib - Open project
 *
 * =====================================================================================
 */

#ifndef ZERO_CROSS_H
#define ZERO_CROSS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZERO_CROSS_DEAPI(T, S) \
int_fast32_t zero_cross_##S (const T* data, uint_fast32_t length, T err)

ZERO_CROSS_DEAPI(int16_t, s);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ZERO_CROSS_H */

