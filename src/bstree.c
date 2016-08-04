// bstree.c

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bstree.h"

struct bsnode {
    void* data;
    struct bsnode* p;
    struct bsnode* left;
    struct bsnode* right;
};

struct bstree {
    struct bsnode *root;
    size_t size;
    compare_t compare;
    pthread_mutex_t mutex;
};


static void bsnode_walk (const struct bsnode* node, void (*callback) (const void* data))
{
    if (NULL != node) {
        bsnode_walk(node->left, callback);
        callback(node->data);
        bsnode_walk(node->right, callback);
    }
}

static struct bsnode* bsnode_search (struct bsnode* node, const void* data, compare_t compare)
{
    int_fast32_t cmp;

    while (NULL != node) {
        if (0 == (cmp = compare(data, node->data))) {
            return node;
        } else if (cmp < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return node;
}

static struct bsnode* bsnode_min (struct bsnode* node)
{
    while (NULL != node->left) {
        node = node->left;
    }

    return node;
}

static struct bsnode* bsnode_max (struct bsnode* node)
{
    while (NULL != node->right) {
        node = node->right;
    }

    return node;
}

/*
static struct bsnode* bsnode_successor (struct bsnode* node)
{
    if (NULL != node->right) {
        return bsnode_min(node->right);
    }

    struct bsnode* p = node->p;
    while ((NULL != p) && (node == p->right)) {
        node = p;
        p = node->p;
    }

    return p;
}

static struct bsnode* bsnode_predecessor (struct bsnode* node)
{
    if (NULL != node->left) {
        return bsnode_max(node->left);
    }

    struct bsnode* p = node->p;
    while ((NULL != p) && (node == p->left)) {
        node = p;
        p = node->p;
    }

    return p;
}
*/

static void bsnode_insert (struct bsnode** root, struct bsnode* node, compare_t compare)
{
    struct bsnode* p = NULL;
    struct bsnode* x = *root;

    while (NULL != x) {
        p = x;
        if (compare(node->data, x->data) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node->p = p;
    if (NULL == p) {
        *root = node;
    } else if (compare(node->data, p->data) < 0) {
        p->left = node;
    } else {
        p->right = node;
    }
}

static void bsnode_transplant (struct bsnode** root, struct bsnode* u, struct bsnode* v)
{
    if (NULL == u->p) {
        *root = v;
    } else if (u == u->p->left) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }

    if (NULL != v) {
        v->p = u->p;
    }
}

static void bsnode_delete (struct bsnode** root, struct bsnode* node)
{
    if (NULL == node->left) {
        bsnode_transplant(root, node, node->right);
    } else if (NULL == node->right) {
        bsnode_transplant(root, node, node->left);
    } else {
        struct bsnode* y = bsnode_min(node->right);
        if (node != y->p) {
            bsnode_transplant(root, y, y->right);
            y->right = node->right;
            y->right->p = y;
        }
        bsnode_transplant(root, node, y);
        y->left = node->left;
        y->left->p = y;
    }
}

static void bsnode_destroy (struct bsnode* node)
{
    if (NULL != node) {
        bsnode_destroy(node->left);
        bsnode_destroy(node->right);
        free(node->data);
        free(node);
    }
}


struct bstree* bstree_create (size_t size, compare_t compare)
{
    if ((0 == size) || (NULL == compare)) {
        return NULL;
    }

    struct bstree* bstree = malloc(sizeof(struct bstree));

    if (NULL == bstree) {
        return NULL;
    }

    if (pthread_mutex_init(&bstree->mutex, NULL)) {
        free(bstree);
        return NULL;
    }

    bstree->compare = compare;
    bstree->size = size;
    bstree->root = NULL;

    return bstree;
}

int_fast32_t bstree_walk (struct bstree* bstree, void (*callback) (const void* data))
{
    if ((NULL == bstree) || (NULL == callback)) {
        return -1;
    }

    pthread_mutex_lock(&bstree->mutex);
    bsnode_walk(bstree->root, callback);
    pthread_mutex_unlock(&bstree->mutex);

    return 0;
}

int_fast32_t bstree_search (struct bstree* bstree, void* data)
{
    if ((NULL == bstree) || (NULL == data)) {
        return -1;
    }

    int_fast32_t result = -1;

    pthread_mutex_lock(&bstree->mutex);
    struct bsnode* node = bsnode_search(bstree->root, data, bstree->compare);
    if (NULL != node) {
        memcpy(data, node->data, bstree->size);
        result = 0;
    }
    pthread_mutex_unlock(&bstree->mutex);

    return result;
}

int_fast32_t bstree_min (struct bstree* bstree, void* data)
{
    if ((NULL == bstree) || (NULL == data)) {
        return -1;
    }

    pthread_mutex_lock(&bstree->mutex);
    if (NULL == bstree->root) {
        pthread_mutex_unlock(&bstree->mutex);
        return -1;
    }
    memcpy(data, bsnode_min(bstree->root)->data, bstree->size);
    pthread_mutex_unlock(&bstree->mutex);

    return 0;
}

int_fast32_t bstree_max (struct bstree* bstree, void* data)
{
    if ((NULL == bstree) || (NULL == data)) {
        return -1;
    }

    pthread_mutex_lock(&bstree->mutex);
    if (NULL == bstree->root) {
        pthread_mutex_unlock(&bstree->mutex);
        return -1;
    }
    memcpy(data, bsnode_max(bstree->root)->data, bstree->size);
    pthread_mutex_unlock(&bstree->mutex);

    return 0;
}

int_fast32_t bstree_insert (struct bstree* bstree, const void* data)
{
    if ((NULL == bstree) || (NULL == data)) {
        return -1;
    }

    struct bsnode* node = malloc(sizeof(struct bsnode));
    if (NULL == node) {
        return -1;
    }
    if (NULL == (node->data = malloc(bstree->size))) {
        free(node);
        return -1;
    }

    memcpy(node->data, data, bstree->size);
    node->left = NULL;
    node->right = NULL;

    pthread_mutex_lock(&bstree->mutex);
    bsnode_insert(&bstree->root, node, bstree->compare);
    pthread_mutex_unlock(&bstree->mutex);

    return 0;
}

int_fast32_t bstree_delete (struct bstree* bstree, void* data)
{
    if ((NULL == bstree) || (NULL == data)) {
        return -1;
    }

    pthread_mutex_lock(&bstree->mutex);
    struct bsnode* node = bsnode_search(bstree->root, data, bstree->compare);
    if (NULL == node) {
        pthread_mutex_unlock(&bstree->mutex);
        return -1;
    }
    bsnode_delete(&bstree->root, node);
    pthread_mutex_unlock(&bstree->mutex);

    memcpy(data, node->data, bstree->size);
    free(node->data);
    free(node);

    return 0;
}

void bstree_destroy (struct bstree* bstree)
{
    if (NULL == bstree) {
        return;
    }

    bsnode_destroy(bstree->root);
    pthread_mutex_destroy(&bstree->mutex);
    free(bstree);
}

