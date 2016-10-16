/*
 * =====================================================================================
 *
 *       Filename:  tzero-cross.c
 *
 *    Description:  测试 zero-cross
 *
 *        Version:  1.0
 *        Created:  2016年10月16日 18:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  tanhuacheng (tanhc), tanhc.gz@gmail.com
 *   Organization:  tanlib - Open Project
 *
 * =====================================================================================
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#include "zero-cross.h"


#define PI 3.1415926535897932384626433832795


static int16_t data[160] = {0};

static void test (int frq)
{
    for (int i = 0; i < 160; i++) {
        data[i] = 32767 * sin(((2*PI*frq*i)/8000) + (PI / 4));
    }

    printf("%4d@8000 with 160: %" PRIdFAST32 "\n", frq, zero_cross_s(data, 160, 0));
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    assert(zero_cross_s(NULL, 0, -1) < 0);
    assert(zero_cross_s(data, 160, 0) >= 0);

    test(50);
    test(400);
    test(500);
    test(600);
    test(800);
    test(900);
    test(1000);
    test(2000);
    test(4000);

    return 0;
}

