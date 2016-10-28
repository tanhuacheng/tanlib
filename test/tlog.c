// tlog.c
// 测试 log(记录日志消息)

/* 包含头文件 ------------------------------------------------------------------------------------*/
#include <assert.h>
#include "log.h"

/* 函数定义 --------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    assert(LOGE("test") >= 0);
    assert(LOGE("test: %d\n", 1) >= 0);

    assert(LOGW("test\n") >= 0);
    assert(LOGW("test: %d\n", 1) >= 0);

    assert(LOGI("test\n") >= 0);
    assert(LOGI("test: %d\n", 1) >= 0);

    assert(LOG_NEWLINE() >= 0);

    return 0;
}

/****************************************** end of file *******************************************/
