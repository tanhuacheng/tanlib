/*
*********************************************************************************************************
* 文件: queue-list.c
* 版本: V0.01
* 创建: Mon Jun  6 10:32:19 2016
* 作者: 谭化成
* 描述: 列队(链表实现)
*********************************************************************************************************
*/

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "queue-list.h"

/* 私有宏定义 -----------------------------------------------------------------------------------------*/

/* 私有数据类型声明 -----------------------------------------------------------------------------------*/
typedef struct queue_list_node {
    void* buffer;
    uint16_t size;
    struct queue_list_node* next;
} queue_list_node_t;

struct queue_list {
    volatile queue_list_node_t* head;
    volatile queue_list_node_t* tail;
    volatile uint32_t wait;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

/* 私有全局变量定义 -----------------------------------------------------------------------------------*/

/* 私有函数声明 ---------------------------------------------------------------------------------------*/

/* 函数定义 -------------------------------------------------------------------------------------------*/
/* {{{ queue_list_create */
queue_list_t* queue_list_create (void)
{
    queue_list_t* ql;
    queue_list_node_t* node;

    if (NULL == (ql = malloc(sizeof(queue_list_t)))) {
        return NULL;
    }
    if (NULL == (node = malloc(sizeof(queue_list_node_t)))) {
        goto error_free_ql;
    }
    if (pthread_cond_init(&ql->cond, NULL)) {
        goto error_free_node;
    }
    if (pthread_mutex_init(&ql->mutex, NULL)) {
        goto error_destroy_cond;
    }

    node->buffer = NULL;
    node->size = 0;
    node->next = NULL;
    ql->head = node;
    ql->tail = node;
    ql->wait = 0;

    return ql;


error_destroy_cond:
    pthread_cond_destroy(&ql->cond);
error_free_node:
    free(node);
error_free_ql:
    free(ql);

    return NULL;
}
/* }}} */

/* {{{ queue_list_read */
// timeout: <0 = 立即返回, 0 = 一直等待, >0 = 超时时间(ms)
int32_t queue_list_read (queue_list_t* ql, void* buffer, uint16_t limit, int32_t timeout)
{
    if ((NULL == ql) || (NULL == buffer)) {
        return -1;
    }

    pthread_mutex_lock(&ql->mutex);
    if (NULL == ql->head->next) {
        if (0 > timeout) {
            pthread_mutex_unlock(&ql->mutex);
            return 0;
        }
        int32_t rc = 0;
        ql->wait++;
        if (0 == timeout) {
            rc = pthread_cond_wait(&ql->cond, &ql->mutex);
        } else {
            struct timespec ts;
            time_t sec, nsc;

            sec = (timeout / 1000);
            nsc = (timeout % 1000) * 1000000;
            clock_gettime(CLOCK_REALTIME, &ts);
            nsc += ts.tv_nsec;
            sec += (nsc / 1000000000);
            nsc %= 1000000000;
            ts.tv_nsec = nsc;
            ts.tv_sec += sec;
            rc = pthread_cond_timedwait(&ql->cond, &ql->mutex, &ts);
        }
        if (rc) {
            ql->wait--;
        }
    }
    if (NULL == ql->head->next) { // 等待超时 或 等待过程中调用了flush
        pthread_mutex_unlock(&ql->mutex);
        return 0;
    }
    queue_list_node_t* node = ql->head->next;
    limit = limit < node->size ? limit : node->size;
    memcpy(buffer, node->buffer, limit);
    ql->head->next = node->next;
    if (NULL == ql->head->next) {
        ql->tail = ql->head;
    }
    free(node->buffer);
    free(node);
    pthread_mutex_unlock(&ql->mutex);

    return limit;
}
/* }}} */

/* {{{ queue_list_write */
int32_t queue_list_write (queue_list_t* ql, const void* buffer, uint16_t size)
{
    if ((NULL == ql) || (NULL == buffer) || (0 == size)) {
        return -1;
    }

    queue_list_node_t* node = malloc(sizeof(queue_list_node_t));
    if (NULL == node) {
        return -1;
    }
    if (NULL == (node->buffer = malloc(size))) {
        free(node);
        return -1;
    }

    memcpy(node->buffer, buffer, size);
    node->size = size;
    node->next = NULL;

    pthread_mutex_lock(&ql->mutex);
    ql->tail->next = node;
    ql->tail = node;
    if (ql->wait) {
        if (0 == pthread_cond_signal(&ql->cond)) {
            ql->wait--;
        }
    }
    pthread_mutex_unlock(&ql->mutex);

    return size;
}
/* }}} */

/* {{{ queue_list_flush */
int32_t queue_list_flush (queue_list_t* ql)
{
    if (NULL == ql) {
        return -1;
    }

    queue_list_node_t* node;

    pthread_mutex_lock(&ql->mutex);
    while (NULL != ql->head->next) {
        node = ql->head->next;
        ql->head->next = node->next;
        free(node->buffer);
        free(node);
    }
    ql->tail = ql->head;
    if (ql->wait) {
        pthread_cond_broadcast(&ql->cond);
        ql->wait = 0;
    }
    pthread_mutex_unlock(&ql->mutex);

    return 0;
}
/* }}} */

/* {{{ queue_list_destroy */
// 须保证销毁ql之时和之后没有任何其它线程使用它
int32_t queue_list_destroy (queue_list_t* ql)
{
    if (NULL == ql) {
        return -1;
    }

    queue_list_flush(ql);
    pthread_cond_destroy(&ql->cond);
    pthread_mutex_destroy(&ql->mutex);
    free((void*)ql->head);
    free((void*)ql);

    return 0;
}
/* }}} */

/********************************************* end of file *********************************************/

