/*
*********************************************************************************************************
* 文件: queue-link.c
* 版本: V0.03
* 创建: Tue Jun  7 21:21:37 2016
* 作者: 谭化成
* 描述: 列队(链表实现, 固定数据长度)
*********************************************************************************************************
*/

/* 包含头文件 -----------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "queue-link.h"

/* 私有宏定义 -----------------------------------------------------------------------------------------*/

/* 私有数据类型声明 -----------------------------------------------------------------------------------*/
typedef struct queue_link_node {
    void* buff;
    struct queue_link_node* next;
} queue_link_node_t;

typedef struct {
    queue_link_node_t* top;
    uint32_t size;
} stack_idle_t;

struct queue_link {
    queue_link_node_t* head;
    queue_link_node_t* tail;
    stack_idle_t idle;
    volatile uint32_t wait;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

/* 私有全局变量定义 -----------------------------------------------------------------------------------*/

/* 私有函数声明 ---------------------------------------------------------------------------------------*/

/* 函数定义 -------------------------------------------------------------------------------------------*/
/* {{{ stack_idle_init */
static inline void stack_idle_init (stack_idle_t* idle, const uint32_t size)
{
    idle->top = NULL;
    idle->size = size;
}
/* }}} */

/* {{{ stack_idle_pop */
// 有则出栈, 空则创建一个
static inline queue_link_node_t* stack_idle_pop (stack_idle_t* idle)
{
    queue_link_node_t* node;

    if (NULL == idle->top) {
        if (NULL == (node = malloc(sizeof(queue_link_node_t)))) {
            return NULL;
        }
        if (NULL == (node->buff = malloc(idle->size))) {
            free(node);
            return NULL;
        }
    } else {
        node = idle->top;
        idle->top = node->next;
    }

    return node;
}
/* }}} */

/* {{{ stack_idle_push */
static inline void stack_idle_push (stack_idle_t* idle, queue_link_node_t* node)
{
    node->next = idle->top;
    idle->top = node;
}
/* }}} */

/* {{{ stack_idle_free */
static inline void stack_idle_free (stack_idle_t* idle)
{
    queue_link_node_t* node;

    while (NULL != idle->top) {
        node = idle->top;
        idle->top = node->next;
        free(node->buff);
        free(node);
    }
}
/* }}} */

/* {{{ queue_link_create */
queue_link_t* queue_link_create (const int32_t size)
{
    queue_link_t* ql;
    queue_link_node_t* node;

    if (0 >= size) {
        return NULL;
    }

    if (NULL == (ql = malloc(sizeof(queue_link_t)))) {
        return NULL;
    }
    if (NULL == (node = malloc(sizeof(queue_link_node_t)))) {
        goto error_free_ql;
    }
    if (pthread_cond_init(&ql->cond, NULL)) {
        goto error_free_node;
    }
    if (pthread_mutex_init(&ql->mutex, NULL)) {
        goto error_destroy_cond;
    }

    stack_idle_init(&ql->idle, size);
    node->buff = NULL;
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

/* {{{ spec_gettimeout */
static inline void spec_gettimeout (struct timespec* ts, const uint32_t timeout)
{
    time_t sec, nsc;

    sec = (timeout / 1000);
    nsc = (timeout % 1000) * 1000000;
    clock_gettime(CLOCK_REALTIME, ts);
    nsc += ts->tv_nsec;
    sec += (nsc / 1000000000);
    nsc %= 1000000000;
    ts->tv_nsec = nsc;
    ts->tv_sec += sec;
}
/* }}} */

/* {{{ queue_link_read */
// timeout: <0 = 立即返回, 0 = 一直等待, >0 = 超时时间(ms)
int32_t queue_link_read (queue_link_t* ql, void* buff, const int32_t timeout)
{
    if ((NULL == ql) || (NULL == buff)) {
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
            spec_gettimeout(&ts, timeout);
            rc = pthread_cond_timedwait(&ql->cond, &ql->mutex, &ts);
        }
        if (rc) { // 等待条件变量超时或错误, 否则由触发者设置
            ql->wait--;
        }
    }

    // 等待超时或等待过程中调用了flush, ql->head->next 可能已经改变
    queue_link_node_t* node = ((volatile queue_link_node_t*)ql->head)->next;
    if (NULL == node) {
        pthread_mutex_unlock(&ql->mutex);
        return 0;
    }

    memcpy(buff, node->buff, ql->idle.size);
    ql->head->next = node->next;
    if (NULL == node->next) {
        ql->tail = ql->head;
    }
    stack_idle_push(&ql->idle, node);
    pthread_mutex_unlock(&ql->mutex);

    return ql->idle.size;
}
/* }}} */

/* {{{ queue_link_write */
int32_t queue_link_write (queue_link_t* ql, const void* buff)
{
    if ((NULL == ql) || (NULL == buff)) {
        return -1;
    }

    queue_link_node_t* node;

    pthread_mutex_lock(&ql->mutex);
    if (NULL == (node = stack_idle_pop(&ql->idle))) {
        pthread_mutex_unlock(&ql->mutex);
        return -1;
    }
    memcpy(node->buff, buff, ql->idle.size);
    node->next = NULL;
    ql->tail->next = node;
    ql->tail = node;
    if (ql->wait) {
        if (0 == pthread_cond_signal(&ql->cond)) {
            ql->wait--;
        }
    }
    pthread_mutex_unlock(&ql->mutex);

    return ql->idle.size;
}
/* }}} */

/* {{{ queue_link_flush */
int32_t queue_link_flush (queue_link_t* ql)
{
    if (NULL == ql) {
        return -1;
    }

    queue_link_node_t* node;

    pthread_mutex_lock(&ql->mutex);
    while (NULL != ql->head->next) {
        node = ql->head->next;
        ql->head->next = node->next;
        stack_idle_push(&ql->idle, node);
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

/* {{{ queue_link_destroy */
// 须保证销毁ql之时和之后没有任何其它线程使用它
int32_t queue_link_destroy (queue_link_t* ql)
{
    if (NULL == ql) {
        return -1;
    }

    queue_link_flush(ql);
    stack_idle_free(&ql->idle);
    pthread_cond_destroy(&ql->cond);
    pthread_mutex_destroy(&ql->mutex);
    free(ql->head);
    free(ql);

    return 0;
}
/* }}} */

/********************************************* end of file *********************************************/

