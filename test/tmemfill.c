#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "memfill.h"

#define ARRAY_LENGTH 1024

struct data {
    int x;
    int y;
};

static struct data array[ARRAY_LENGTH] = {{0}};

static void show_array (void)
{
    printf("\n");
    printf("hexview the fisrt 5 of array in format {.x = ?, .y = ?}\n");
    for (int i = 0; i < 5; i++) {
        printf("{.x = %08x, .y = %08x}\n", array[i].x, array[i].y);
    }
    printf("end\n");
    printf("\n");
}

static void* memfill2 (void* restrict dstp, size_t dstsize, const void* restrict patp, size_t patsize)
{
    if ((NULL == dstp) || (NULL == patp)) {
        return dstp;
    }

    for (; dstsize > patsize; dstsize -= patsize) {
        memcpy(dstp, patp, patsize);
        dstp += patsize;
    }
    memcpy(dstp, patp, dstsize);

    return dstp;
}

int main (int argc, char* argv[])
{
    struct data d;

    d.x = 0x55;
    d.y = 0xaa;

    printf("memset array with 0x55:\n");
    memset(array, 0x55, sizeof(array));
    show_array();

    printf("memfill array with struct data {.x = 0x55, .y = 0xaa}:\n");
    memfill(array, sizeof(array), &d, sizeof(d));
    show_array();

    printf("memfill2 array with struct data {.x = 0x55, .y = 0xaa}:\n");
    memset(array, 0, sizeof(array));
    memfill2(array, sizeof(array), &d, sizeof(d));
    show_array();

    struct timeval tv_b, tv_e;
    int64_t time_use;
    int i;

    gettimeofday(&tv_b, NULL);
    for (i = 0; i < 1000000; i++) {
        memfill(array, sizeof(array), &d, sizeof(d));
    }
    gettimeofday(&tv_e, NULL);
    time_use = ((tv_e.tv_sec - tv_b.tv_sec) * 1000) + ((tv_e.tv_usec - tv_b.tv_usec) / 1000);
    printf("memfill  timeuse = %ld\n", time_use);

    gettimeofday(&tv_b, NULL);
    for (i = 0; i < 1000000; i++) {
        memfill2(array, sizeof(array), &d, sizeof(d));
    }
    gettimeofday(&tv_e, NULL);
    time_use = ((tv_e.tv_sec - tv_b.tv_sec) * 1000) + ((tv_e.tv_usec - tv_b.tv_usec) / 1000);
    printf("memfill2 timeuse = %ld\n", time_use);

    return 0;
}

/*
memset array with 0x55:

hexview the fisrt 5 of array in format {.x = ?, .y = ?}
{.x = 55555555, .y = 55555555}
{.x = 55555555, .y = 55555555}
{.x = 55555555, .y = 55555555}
{.x = 55555555, .y = 55555555}
{.x = 55555555, .y = 55555555}
end

memfill array with struct data {.x = 0x55, .y = 0xaa}:

hexview the fisrt 5 of array in format {.x = ?, .y = ?}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
end

memfill2 array with struct data {.x = 0x55, .y = 0xaa}:

hexview the fisrt 5 of array in format {.x = ?, .y = ?}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
{.x = 00000055, .y = 000000aa}
end

memfill  timeuse = 1421
memfill2 timeuse = 15487
*/

