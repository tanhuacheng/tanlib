#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "queue-link.h"

#define BUFFER_SIZE 160

static int16_t buffer[BUFFER_SIZE];
static int16_t tempbf[BUFFER_SIZE];
static queue_link_t* ql;

static void* thread_send (void*);

int main (int argc, char* argv[])
{
    if (NULL == (ql = queue_link_create(BUFFER_SIZE * sizeof(int16_t)))) {
        printf("error line %d\n", __LINE__);
        return 0;
    }

    // write
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = i;
    }
    if (queue_link_write(ql, buffer) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    if (queue_link_write(ql, buffer) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    if (queue_link_write(ql, buffer) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }

    // read
    int32_t res;
    memset(tempbf, 0, sizeof(tempbf));
    if ((res = queue_link_read(ql, tempbf, -1)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);
    res /= sizeof(tempbf[0]);
    for (int i = 0; i < res; i++) {
        if (i != tempbf[i]) {
            printf("error line %d\n", __LINE__);
            return 0;
        }
    }
    memset(tempbf, 0, sizeof(tempbf));
    if ((res = queue_link_read(ql, tempbf, 0)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);
    res /= sizeof(tempbf[0]);
    for (int i = 0; i < res; i++) {
        if (i != tempbf[i]) {
            printf("error line %d\n", __LINE__);
            return 0;
        }
    }
    memset(tempbf, 0, sizeof(tempbf));
    if ((res = queue_link_read(ql, tempbf, 2000)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);
    res /= sizeof(tempbf[0]);
    for (int i = 0; i < res; i++) {
        if (i != tempbf[i]) {
            printf("error line %d\n", __LINE__);
            return 0;
        }
    }

    // wait
    if ((res = queue_link_read(ql, tempbf, -1)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);
    if ((res = queue_link_read(ql, tempbf, 3000)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);

    /*
    if ((res = queue_link_read(ql, tempbf, 0)) < 0) {
        printf("error line %d\n", __LINE__);
        return 0;
    }
    printf("line %d, res = %d\n", __LINE__, res);
    */

    pthread_t tid;
    pthread_create(&tid, NULL, thread_send, NULL);

    uint16_t cnt = 0;
    while (true) {
        memset(tempbf, 0, sizeof(tempbf));
        if ((res = queue_link_read(ql, tempbf, 0)) < 0) {
            printf("error line %d\n", __LINE__);
            return 0;
        }
        printf("line %d, res = %d\n", __LINE__, res);
        if (res > 0) {
            if (tempbf[0] != (int16_t)cnt++) {
                printf("error line %d\n", __LINE__);
                return 0;
            }
        }
        res /= sizeof(tempbf[0]);
        for (int i = 1; i < res; i++) {
            if (i != tempbf[i]) {
                printf("error line %d\n", __LINE__);
                return 0;
            }
        }
    }

    return 0;
}

static void* thread_send (void* args)
{
    uint16_t cnt = 0;
    args = args;

    while (true) {
        buffer[0] = (int16_t)cnt++;
        if (queue_link_write(ql, buffer) < 0) {
            printf("error line %d\n", __LINE__);
            return NULL;
        }
        sleep(1);
    }

    return NULL;
}
