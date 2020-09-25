#include <stdio.h>
#include <stdlib.h>
#include "hash_dict.h"
#include "hash.h"

#define HASH_BUCKET(this, hash)     ((hash) % (this)->buckets)
#define HASH_ITEM(this, bucket)     (&(this)->items[(bucket)])

static size_t _bucket_advance(long *hash, size_t bucket, size_t buckets) {
    /* Python dict algorithm */
    size_t result = 5 * bucket + *hash + 1;
    *hash >>= 5;
    return result % buckets;
}

static size_t _next_hash_bucket(struct hash_dict *this, long hash) {
    size_t bucket = HASH_BUCKET(this, hash);
    while(HASH_ITEM(this, bucket)->key)
        bucket = _bucket_advance(&hash, bucket, this->buckets);
    return bucket;
}

static void _hash_dict_add(struct hash_dict *this, long hash, const char *key, const char *value) {
    size_t bucket = _next_hash_bucket(this, hash);
    HASH_ITEM(this, bucket)->hash = hash;
    HASH_ITEM(this, bucket)->key = key;
    HASH_ITEM(this, bucket)->value = value;
    ++this->count;
}

struct hash_dict *hash_dict_new(size_t buckets) {
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
    size_t new_count = (*this)->buckets * 2;
    if((*this)->buckets > new_count)
        return;
    struct hash_dict *new = hash_dict_new(new_count);
    for(size_t i = 0; i < (*this)->buckets; ++i) {
        if(!HASH_ITEM(*this, i)->key)
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
    long hash = STRING_HASH(key);
    _hash_dict_add(*this, hash, key, value);
}

const char *hash_dict_get(struct hash_dict *this, const char *key) {
    long hash = STRING_HASH(key);
    size_t bucket = HASH_BUCKET(this, hash);
    while(HASH_ITEM(this, bucket)->key) {
        if(strcmp(HASH_ITEM(this, bucket)->key, key) == 0)
            return HASH_ITEM(this, bucket)->value;
        bucket = _bucket_advance(&hash, bucket, this->buckets);
    }
    return NULL;
}

void hash_dict_print(struct hash_dict *this) {
    printf("{\n");
    for(size_t i = 0; i < this->buckets; ++i) {
        if(!HASH_ITEM(this, i)->key) {
            continue;
        }
        printf("  %lu.) %s: %s\n",
            i,
            HASH_ITEM(this, i)->key,
            HASH_ITEM(this, i)->value);
    }
    printf("}\n");
}
