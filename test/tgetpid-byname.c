// tgetpid-byname.c
// 测试 getpid-byname

/* 包含头文件 ------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "getpid-byname.h"

/* 函数定义 --------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char* namep = NULL;

    if (argc > 1) {
        namep = argv[1];
    } else {
        char* p = argv[0];
        namep = p;
        for (;;) {
            p = memchr(p, '/', strlen(p));
            if (NULL == p) {
                break;
            }
            namep = ++p;
        }
    }

    printf("pid: %d\n", getpid());
    printf("skip self, getpid_byname: %d\n", getpid_byname(namep, getpid()));
    printf("getpid_byname: %d\n", getpid_byname(namep, 0));

    return 0;
}

/****************************************** end of file *******************************************/
