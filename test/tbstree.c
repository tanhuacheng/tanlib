// tbstree.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "bstree.h"

struct node {
    uint32_t key;
    char string[32];
};

static int compare (const void* data1, const void* data2)
{
    const struct node* d1 = (const struct node*)data1;
    const struct node* d2 = (const struct node*)data2;

    if (d1->key < d2->key) {
        return -1;
    } else if (d1->key == d2->key) {
        return 0;
    }

    return 1;
}

static void callback (const void* data)
{
    const struct node* d = (const struct node*)data;
    printf("key: %d, %s\n", d->key, d->string);
}

static struct bstree* bstree;

static void* thread_insert (void* args)
{
    args = args;
    struct node node;

    while (1) {
        node.key = rand() % 50;
        sprintf(node.string, "%d", node.key);
        bstree_insert(bstree, &node);
        sleep(1);
    }

    return NULL;
}

int main (int argc, char* argv[])
{
    bstree = bstree_create(sizeof(struct node), compare);

    bstree_insert(bstree, &(struct node){key:15, "15"});
    bstree_insert(bstree, &(struct node){key:6, "6"});
    bstree_insert(bstree, &(struct node){key:3, "3"});
    bstree_insert(bstree, &(struct node){key:2, "2"});
    bstree_insert(bstree, &(struct node){key:4, "4"});
    bstree_insert(bstree, &(struct node){key:7, "7"});
    bstree_insert(bstree, &(struct node){key:13, "13"});
    bstree_insert(bstree, &(struct node){key:9, "9"});
    bstree_insert(bstree, &(struct node){key:18, "18"});
    bstree_insert(bstree, &(struct node){key:17, "17"});
    bstree_insert(bstree, &(struct node){key:20, "20"});

    printf("\nwalk:\n");
    bstree_walk(bstree, callback);

    struct node node;

    printf("\nmin:\n");
    bstree_min(bstree, &node);
    callback(&node);

    printf("\nmax:\n");
    bstree_max(bstree, &node);
    callback(&node);

    printf("\nsearch 13:\n");
    node.key = 13;
    bstree_search(bstree, &node);
    callback(&node);

    printf("\ndelete 3:\n");
    node.key = 3;
    bstree_delete(bstree, &node);
    callback(&node);

    printf("\nwalk:\n");
    bstree_walk(bstree, callback);
    printf("\n");

    bstree_destroy(bstree);

    bstree = bstree_create(sizeof(struct node), compare);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_insert, NULL);

    while (1) {
        bstree_walk(bstree, callback);
        printf("walk\n");
        sleep(1);
    }

    return 0;
}

