/*
*********************************************************************************************************
* 文件: queue-array.c
* 版本: V0.00
* 创建: Thu May 26 14:08:13 2016
* 作者: 谭化成
* 描述: 列队(数组实现)
*********************************************************************************************************
*/

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <string.h>
#include "incmod.h"
#include "queue-array.h"

/* 私有宏定义 -----------------------------------------------------------------------------------------*/

/* 私有数据类型声明 -----------------------------------------------------------------------------------*/

/* 私有全局变量定义 -----------------------------------------------------------------------------------*/

/* 私有函数声明 ---------------------------------------------------------------------------------------*/

/* 函数定义 -------------------------------------------------------------------------------------------*/
static inline bool queue_array_valid (const queue_array_t *q)
{
    return !((NULL == q->data.buff) || (0 == q->data.size) || (0 == q->data.mlen));
}

static inline uint16_t queue_array_cal_length (
    const uint16_t front,
    const uint16_t rear,
    const uint16_t max,
    const bool full)
{
    if (rear > front) {
        return rear - front;
    } else if (front > rear) {
        return max - (front - rear);
    } else if (full) {
        return max;
    } else {
        return 0;
    }
}

int32_t queue_array_init (queue_array_t *q, void* buff, const size_t size, const uint16_t mlen);
{
    if ((NULL == q) || (NULL == buff) || (0 == size) || (0 == mlen)) {
        return -1;
    }

    memset(buff, 0, size * mlen);

    q->data.buff = buff;
    q->data.size = size;
    q->data.mlen = mlen;
    q->front = 0;
    q->rear = 0;
    q->full = false;

    return 0;
}

int32_t queue_array_read (queue_array_t *q, void *buff, uint16_t length);
{
    uint16_t front, rear, max;
    uint16_t copy;
    size_t size;

    if ((NULL == q) || (NULL == buff)) {
        return -1;
    }

    if (!queue_array_valid(q)) {
        return -1;
    }

    front = q->front;
    rear = q->rear;
    max = q->data.mlen;

    length = queue_array_cal_length(front, rear, max, q->full);
    length = copy < length ? copy : length;

    if (0 == copy) {
        return 0;
    }

    copy = length;
    size = q->data.size;

    if ((front + copy) >= max) {
        uint16_t cp = max - front;
        memcpy(buff, q->data.buff + (front * q->data.size), cp * q->data.size);
        buff += cp * q->data.size;
        copy -= cp;
        front = 0;
    }

    if (copy > 0) {
        memcpy(buff, q->data.buff + (front * q->data.size), copy * q->data.size);
    }


}

int32_t queue_array_write (queue_array_t *q, const queue_array_data_t *datap, uint16_t length);

int32_t queue_array_length (const queue_array_t *q)
{

}

/********************************************* end of file *********************************************/
























