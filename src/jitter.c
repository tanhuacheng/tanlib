/*
*********************************************************************************************************
* 文件: jitter.c
* 版本: V0.01
* 创建: Tue May 31 11:23:49 2016
* 作者: 谭化成
* 描述: 抖动缓冲区
*********************************************************************************************************
*/

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "queue-array.h"
#include "jitter.h"

/* 私有宏定义 -----------------------------------------------------------------------------------------*/

/* 私有数据类型声明 -----------------------------------------------------------------------------------*/
struct jitter {
    struct {
        uint16_t max;
        uint16_t hig;
        uint16_t std;
        uint16_t low;
        uint16_t met;
        uint16_t size;
        uint16_t unit;
    } param;
    queue_array_t queue;
    enum {
        JITTER_STANDBY,
        JITTER_READY,   
    } state;
    uint16_t cnt_drop;
    uint16_t cnt_insert;
    pthread_mutex_t lock;
};

/* 私有全局变量定义 -----------------------------------------------------------------------------------*/

/* 私有函数声明 ---------------------------------------------------------------------------------------*/

/* 函数定义 -------------------------------------------------------------------------------------------*/
// size: 数据单元字节数, unit: 每次读取的数据单元数
jitter_t* jitter_create (const size_t size, const uint16_t unit)
{
    jitter_t* jitter;
    void* buffer;

    if ((0 == size) || (0 == unit)) {
        return NULL;
    }

    if (NULL == (jitter = malloc(sizeof(jitter_t)))) {
        return NULL;
    }

    jitter->param.size = size;
    jitter->param.unit = unit;

    jitter->param.max = unit * JITTER_PARAM_MUL_MAX;
    jitter->param.hig = unit * JITTER_PARAM_MUL_HIG;
    jitter->param.std = unit * JITTER_PARAM_MUL_STD;
    jitter->param.low = unit * JITTER_PARAM_MUL_LOW;
    jitter->param.met = unit * JITTER_PARAM_MUL_MET;

    if (pthread_mutex_init(&jitter->lock, NULL)) {
        goto err_free_jitter;
    }

    if (NULL == (buffer = malloc(size * jitter->param.max))) {
        goto err_destroy_lock;
    }

    if (queue_array_init(&jitter->queue, buffer, size, jitter->param.max) < 0) {
        goto err_free_buffer;
    }

    jitter->state = JITTER_STANDBY;
    jitter->cnt_drop = 0;
    jitter->cnt_insert = 0;

    return jitter;

err_free_buffer:
    free(buffer);
err_destroy_lock:
    pthread_mutex_destroy(&jitter->lock);
err_free_jitter:
    free(jitter);

    return NULL;
}

// 读取长度 = unit; 返回: -1 = 出现错误, 0 = 正常读取, >0 = 补零数量.
int32_t jitter_read (jitter_t* jitter, void* buffer)
{
    int32_t retn = 0;

    if ((NULL == jitter) || (NULL == buffer)) {
        return -1;
    }

    pthread_mutex_lock(&jitter->lock);
    memset(buffer, 0, jitter->param.size * jitter->param.unit);

    if (JITTER_READY == jitter->state) {
        int32_t length = queue_array_length(&jitter->queue);
        int32_t read = jitter->param.unit;

        if (length < 0) {
            pthread_mutex_unlock(&jitter->lock);
            return -1;
        }

        if (length < jitter->param.unit) {
            jitter->state = JITTER_STANDBY;
            jitter->cnt_insert = 0;
            pthread_mutex_unlock(&jitter->lock);
            return -1;
        }

        if (length < jitter->param.low) {
            jitter->cnt_insert += jitter->param.unit;
            if (jitter->cnt_insert >= jitter->param.met) {
                jitter->cnt_insert = 0;
                read -= 1;
                retn += 1;
            }
        } else {
            jitter->cnt_insert = 0;
        }

        int res;

        if ((res = queue_array_read(&jitter->queue, buffer, read)) < 0) {
            jitter->cnt_insert = 0;
            pthread_mutex_unlock(&jitter->lock);
            return -1;
        }
        retn += read - res;
        if ((length - res) < jitter->param.unit) {
            jitter->state = JITTER_STANDBY;
            jitter->cnt_insert = 0;
        }
    } else {
        retn += jitter->param.unit;
    }

    pthread_mutex_unlock(&jitter->lock);

    return retn;
}

// 返回: -1 = 出现错误, 0 = 正常写入, >0 = 丢弃数量.
int32_t jitter_write (jitter_t* jitter, const void* buffer, uint16_t length)
{
    int32_t buffer_length;
    int32_t retn = 0;

    if ((NULL == jitter) || (NULL == buffer)) {
        return -1;
    }

    pthread_mutex_lock(&jitter->lock);
    if ((buffer_length = queue_array_length(&jitter->queue)) < 0) {
        pthread_mutex_unlock(&jitter->lock);
        return -1;
    }

    if (buffer_length >= jitter->param.hig) {
        jitter->cnt_drop += buffer_length;
        if (jitter->cnt_drop >= jitter->param.met) {
            jitter->cnt_drop = 0;
            length -= 1;
            retn += 1;
        }
    } else {
        jitter->cnt_drop = 0;
    }

    int32_t res;

    if ((res = queue_array_write(&jitter->queue, buffer, length)) < 0) {
        jitter->cnt_drop = 0;
        pthread_mutex_unlock(&jitter->lock);
        return -1;
    }
    retn += length - res;
    if ((buffer_length + res) >= jitter->param.std) {
        jitter->state = JITTER_READY;
    }
    pthread_mutex_unlock(&jitter->lock);

    return retn;
}

int32_t jitter_flush (jitter_t* jitter)
{
    if (NULL == jitter) {
        return -1;
    }

    pthread_mutex_lock(&jitter->lock);
    jitter->state = JITTER_STANDBY;
    jitter->cnt_drop = 0;
    jitter->cnt_insert = 0;
    if (queue_array_flush(&jitter->queue) < 0) {
        pthread_mutex_unlock(&jitter->lock);
        return -1;
    }
    pthread_mutex_unlock(&jitter->lock);

    return 0;
}

int32_t jitter_destroy (jitter_t* jitter)
{
    if (NULL == jitter) {
        return -1;
    }

    free(jitter->queue.data.buff);
    if (pthread_mutex_destroy(&jitter->lock) != 0) {
        free(jitter);
        return -1;
    }
    free(jitter);

    return 0;
}

/********************************************* end of file *********************************************/

