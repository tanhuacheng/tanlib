/*
 * =====================================================================================
 *
 *       Filename:  zero-cross.c
 *
 *    Description:  计算信号的短时过零数
 *
 *        Version:  1.0
 *        Created:  2016年10月16日 17:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  tanhuacheng (tanhc), tanhc.gz@gmail.com
 *   Organization:  tanlib - Open project
 *
 * =====================================================================================
 */

#include <stddef.h>
#include <stdbool.h>

#include "zero-cross.h"

#define ZERO_CROSS_DEFUN(T, S) \
ZERO_CROSS_DEAPI(T, S) { \
    if ((NULL == data) || (0 == length) || (err < 0)) { \
        return -1; \
    } \
\
    uint_fast32_t nzc = 0; \
    bool sig = data[0] < 0; \
\
    for (uint_fast32_t i = 1; i < length; i++) { \
        if (sig) { \
            if (data[i] > err) { \
                nzc++; \
                sig = false; \
            } \
        } else { \
            if (data[i] < -err) { \
                nzc++; \
                sig = true; \
            } \
        } \
    } \
\
    return nzc; \
}

ZERO_CROSS_DEFUN(int16_t, s)

