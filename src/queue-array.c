/*
*********************************************************************************************************
* 文件: queue-array.c
* 版本: V0.01
* 创建: Thu May 26 14:08:13 2016
* 作者: 谭化成
* 描述: 列队(数组实现)
*********************************************************************************************************
*/

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <string.h>
#include "memfill.h"
#include "queue-array.h"

/* 私有宏定义 -----------------------------------------------------------------------------------------*/

/* 私有数据类型声明 -----------------------------------------------------------------------------------*/

/* 私有全局变量定义 -----------------------------------------------------------------------------------*/

/* 私有函数声明 ---------------------------------------------------------------------------------------*/

/* 函数定义 -------------------------------------------------------------------------------------------*/
static inline bool queue_array_valid (const queue_array_t* q)
{
    if (NULL == q) {
        return false;
    }

    return !((NULL == q->data.buff) || (0 == q->data.size) || (0 == q->data.mlen));
}

static inline uint16_t queue_array_cal_length (
    const uint16_t front,
    const uint16_t rear,
    const uint16_t mlen)
{
    if (rear >= front) {
        return rear - front;
    } else {
        return mlen - (front - rear);
    }
}

int32_t queue_array_init (queue_array_t* q, void* buff, const size_t size, const uint16_t mlen)
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

    return 0;
}

int32_t queue_array_read (queue_array_t* q, void* buff, uint16_t length)
{
    uint16_t front, rear;
    uint16_t copy;

    if ((!queue_array_valid(q)) || (NULL == buff)) {
        return -1;
    }

    front = q->front;
    rear = q->rear;

    copy = queue_array_cal_length(front, rear, q->data.mlen);
    if (0 == (copy = copy < length ? copy : length)) {
        return 0;
    }
    length = copy;

    if ((front + copy) >= q->data.mlen) {
        uint16_t cp = q->data.mlen - front;

        memcpy(buff, q->data.buff + (front * q->data.size), cp * q->data.size);
        buff += cp * q->data.size;
        copy -= cp;
        front = 0;
    }
    if (copy > 0) {
        memcpy(buff, q->data.buff + (front * q->data.size), copy * q->data.size);
        front += copy;
    }

    q->front = front;

    return length;
}

// type: 0 = fill, 1 = write
static int32_t queue_array_fill_write (queue_array_t* q, const void* buff, uint16_t length, const uint8_t type)
{
    uint16_t front, rear;
    uint16_t copy;

    if ((!queue_array_valid(q)) || (NULL == buff) || (1 < type)) {
        return -1;
    }

    front = q->front;
    rear = q->rear;

    copy = q->data.mlen - 1 - queue_array_cal_length(front, rear, q->data.mlen);
    if (0 == (copy = copy < length ? copy : length)) {
        return 0;
    }
    length = copy;

    if ((rear + copy) >= q->data.mlen) {
        uint16_t cp = q->data.mlen - rear;

        if (0 == type) {
            memfill(q->data.buff + (rear * q->data.size), cp * q->data.size, buff, q->data.size);
        } else {
            memcpy(q->data.buff + (rear * q->data.size), buff, cp * q->data.size);
            buff += cp * q->data.size;
        }

        copy -= cp;
        rear = 0;
    }
    if (copy > 0) {
        if (0 == type) {
            memfill(q->data.buff + (rear * q->data.size), copy * q->data.size, buff, q->data.size);
        } else {
            memcpy(q->data.buff + (rear * q->data.size), buff, copy * q->data.size);
        }
        rear += copy;
    }

    q->rear = rear;

    return length;
}

int32_t queue_array_fill (queue_array_t* q, const void* buff, uint16_t length)
{
    return queue_array_fill_write(q, buff, length, 0);
}

int32_t queue_array_write (queue_array_t* q, const void* buff, uint16_t length)
{
    return queue_array_fill_write(q, buff, length, 1);
}

int32_t queue_array_flush (queue_array_t* q)
{
    if (!queue_array_valid(q)) {
        return -1;
    }

    memset(q->data.buff, 0, q->data.size * q->data.mlen);
    q->front = 0;
    q->rear = 0;

    return 0;
}

int32_t queue_array_length (const queue_array_t* q)
{
    if (!queue_array_valid(q)) {
        return -1;
    }

    return queue_array_cal_length(q->front, q->rear, q->data.mlen);
}

/********************************************* end of file *********************************************/

