#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "queue-list.h"
#include "queue-link.h"

#define BUFFER_SIZE 160
#define TIMES       2000000

static queue_list_t* q_list;
static queue_link_t* q_link;

static int16_t buff[BUFFER_SIZE];
static int16_t temp[BUFFER_SIZE];

int main (int argc, char* argv[])
{
    q_list = queue_list_create();
    q_link = queue_link_create(BUFFER_SIZE * sizeof(int16_t));

    struct timeval tv_b, tv_e;
    uint64_t time_use, i;

    // queue-list
    gettimeofday(&tv_b, NULL);
    for (i = 0; i < TIMES; i++) {
        queue_list_write(q_list, buff, sizeof(buff));
        queue_list_read(q_list, temp, sizeof(temp), 0);
    }
    gettimeofday(&tv_e, NULL);

    time_use = ((tv_e.tv_sec - tv_b.tv_sec) * 1000) + ((tv_e.tv_usec - tv_b.tv_usec) / 1000);
    printf("queue-list time-use = %ld\n", time_use);

    // queue-link
    gettimeofday(&tv_b, NULL);
    for (i = 0; i < TIMES; i++) {
        queue_link_write(q_link, buff);
        queue_link_read(q_link, temp, 0);
    }
    gettimeofday(&tv_e, NULL);

    time_use = ((tv_e.tv_sec - tv_b.tv_sec) * 1000) + ((tv_e.tv_usec - tv_b.tv_usec) / 1000);
    printf("queue-link time-use = %ld\n", time_use);

    return 0;
}

/*
 * queue-list time-use = 3051
 * queue-link time-use = 1474
 *
 * */

