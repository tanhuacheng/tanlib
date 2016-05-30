#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "queue-array.h"

#define QUEUE_LENGTH 1024

static int array[QUEUE_LENGTH];

static void show_queue (queue_array_t *q)
{
    int data;

    printf("\n%s begain\n", __func__);
    while (queue_array_read(q, &data, 1) > 0) {
        printf("%d ", data);
    }
    printf("\nend\n");
}

static queue_array_t q;

void* thread_read (void *args)
{
    int buff[60];

    printf("\n");
    while (true) {
        int rd = 0, res;

        while (rd < 60) {
            if ((res = queue_array_read(&q, buff + rd, 60 - rd)) > 0) {
                rd += res;
            } else {
                usleep(20);
            }
        }

        for (int i = 0; i < 60; i++) {
            printf("%d ", buff[i]);
        }
        fflush(stdout);

        sleep(1);
    }
}

int main (int argc, char* argv[])
{
    int data;

    queue_array_init(&q, array, sizeof(int), QUEUE_LENGTH);
    show_queue(&q);

    data = 0x055;
    queue_array_fill(&q, &data, 2);
    show_queue(&q);

    for (int i = 0; i < QUEUE_LENGTH; i++) {
        int res;
        if ((res = queue_array_write(&q, &i, 1)) <= 0) {
            printf("queue_array_write: return = %d, i = %d\n", res, i);
        }
    }
    printf("queue_array_length: %d\n", queue_array_length(&q));
    show_queue(&q);
    printf("queue_array_length: %d\n", queue_array_length(&q));

    pthread_t tid;

    pthread_create(&tid, NULL, thread_read, NULL);

    int buff[100];
    for (int i = 0; i < 100; i++) {
        buff[i] = i;
    }

    // write
    while (true) {
        int wr = 0, res;

        while (wr < 100) {
            if ((res = queue_array_write(&q, buff + wr, 100 - wr)) > 0) {
                wr += res;
            } else {
                usleep(20);
            }
        }

        sleep(2);
    }

    return 0;
}

