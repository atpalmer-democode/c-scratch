#include <stdio.h>
#include <stdlib.h>
#include "hash_dict.h"
#include "hash.h"

#define HASH_BUCKET(this, hash)     ((hash) % (this)->buckets)
#define HASH_ITEM(this, bucket)     (&(this)->items[(bucket)])

static ssize_t _bucket_advance(uint64_t *hash, ssize_t bucket, ssize_t buckets) {
    /* Python dict algorithm */
    ssize_t result = 5 * bucket + *hash + 1;
    *hash >>= 5;
    return result % buckets;
}

static ssize_t _bucket_find(struct hash_dict *this, const char *key) {
    uint64_t hash = STRING_HASH(key);
    ssize_t bucket = HASH_BUCKET(this, hash);
    while(HASH_ITEM(this, bucket)->status != ITEM_STATUS_INACTIVE) {
        if(HASH_ITEM(this, bucket)->status == ITEM_STATUS_ACTIVE
                && strcmp(HASH_ITEM(this, bucket)->key, key) == 0)
            return bucket;
        bucket = _bucket_advance(&hash, bucket, this->buckets);
    }
    return -1;
}

static ssize_t _next_hash_bucket(struct hash_dict *this, uint64_t hash) {
    ssize_t bucket = HASH_BUCKET(this, hash);
    while(HASH_ITEM(this, bucket)->status == ITEM_STATUS_ACTIVE)
        bucket = _bucket_advance(&hash, bucket, this->buckets);
    return bucket;
}

static void _hash_dict_add(struct hash_dict *this, uint64_t hash, const char *key, const char *value) {
    ssize_t bucket = _next_hash_bucket(this, hash);
    HASH_ITEM(this, bucket)->hash = hash;
    HASH_ITEM(this, bucket)->status = ITEM_STATUS_ACTIVE;
    HASH_ITEM(this, bucket)->key = key;
    HASH_ITEM(this, bucket)->value = value;
    ++this->count;
}

struct hash_dict *hash_dict_new(ssize_t buckets) {
    if(!buckets)
        buckets = 1;
    struct hash_dict *new = malloc(sizeof *new + (sizeof(*new->items) * buckets));
    new->count = 0;
    new->buckets = buckets;
    memset(new->items, 0, buckets);
    return new;
}

void hash_dict_free(struct hash_dict *this) {
    free(this);
}

void hash_dict_resize(struct hash_dict **this) {
    ssize_t new_count = (*this)->buckets * 2;
    if((*this)->buckets > new_count)
        return;
    struct hash_dict *new = hash_dict_new(new_count);
    for(ssize_t i = 0; i < (*this)->buckets; ++i) {
        if(HASH_ITEM(*this, i)->status != ITEM_STATUS_ACTIVE)
            continue;
        _hash_dict_add(new,
            HASH_ITEM(*this, i)->hash,
            HASH_ITEM(*this, i)->key,
            HASH_ITEM(*this, i)->value);
    }
    hash_dict_free(*this);
    *this = new;
}

void hash_dict_add(struct hash_dict **this, const char *key, const char *value) {
    hash_dict_resize(&*this);
    uint64_t hash = STRING_HASH(key);
    _hash_dict_add(*this, hash, key, value);
}

void hash_dict_del(struct hash_dict **this, const char *key) {
    /* TODO: resize down when needed */
    ssize_t bucket = _bucket_find(*this, key);
    HASH_ITEM(*this, bucket)->status = ITEM_STATUS_DELETED;
    --(*this)->count;
}

const char *hash_dict_get(struct hash_dict *this, const char *key) {
    ssize_t bucket = _bucket_find(this, key);
    if(bucket < 0)
        return NULL;
    return HASH_ITEM(this, bucket)->value;
}

void hash_dict_print(struct hash_dict *this) {
    printf("{\n");
    for(ssize_t i = 0; i < this->buckets; ++i) {
        if(HASH_ITEM(this, i)->status != ITEM_STATUS_ACTIVE)
            continue;
        printf("  %lu.) %s: %s\n",
            i,
            HASH_ITEM(this, i)->key,
            HASH_ITEM(this, i)->value);
    }
    printf("}\n");
}
