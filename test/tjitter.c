#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "jitter.h"

/*
 * jitter_t* jitter_create (const size_t size, const uint16_t unit);
 * int32_t jitter_read (jitter_t* jitter, void* buffer);
 * int32_t jitter_write (jitter_t* jitter, const void* buffer, uint16_t length);
 * int32_t jitter_flush (jitter_t* jitter);
 * int32_t jitter_destroy (jitter_t* jitter);
 * */

jitter_t *jitter;
void* thread_read (void* args);

int main (int argc, char* argv[])
{
    jitter = jitter_create(sizeof(int16_t), 64);
    if (NULL == jitter) {
        printf ("error: jitter_create\n");
        return 0;
    }

    /*
    if (jitter_flush(jitter) < 0) {
        printf ("error: jitter_flush\n");
        return 0;
    }

    if (jitter_destroy(jitter) < 0) {
        printf ("error: jitter_destroy\n");
        return 0;
    }
    */

    pthread_t tid;
    pthread_create(&tid, NULL, thread_read, NULL);

    int16_t buffer[32];
    int32_t res;

    for (int32_t i = 0; i < 32; i++) {
        buffer[i] = i;
    }

    while (true) {
        if ((res = jitter_write(jitter, buffer, 32)) > 0) {
            printf("write drop %d\n", res);
        }
        if (res < 0) {
            printf("error: jitter_write\n");
        } else {
            printf("write:\n");
        }
        usleep((400 + (rand() % 200) - 100 + 6) * 1000);
    }

    return 0;
}

void* thread_read (void* args)
{
    int16_t buffer[64];
    int32_t res;
//    int32_t i;

    while (true) {
        if ((res = jitter_read(jitter, buffer)) > 0) {
            printf("read insert %d\n", res);
        }
        if (res < 0) {
            printf("error: jitter_read\n");
        } else {
            printf("read:\n");
            /*
            for (i = 0; i < 64; i++) {
                printf("%d ", buffer[i]);
            }
            printf("\n");
            */
        }
        usleep(800 * 1000);
    }

    return NULL;
}

