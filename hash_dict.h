#ifndef HASH_DICT_H
#define HASH_DICT_H

#include <stdint.h>

enum {
    ITEM_STATUS_INACTIVE = 0,
    ITEM_STATUS_ACTIVE,
    ITEM_STATUS_DELETED,
};

struct hash_item {
    uint64_t hash;
    uint8_t status;
    const char *key;
    const char *value;
};

struct hash_dict {
    ssize_t count;
    ssize_t buckets;
    struct hash_item items[];
};

struct hash_dict *hash_dict_new(ssize_t buckets);
void hash_dict_free(struct hash_dict *this);
void hash_dict_resize(struct hash_dict **this);
void hash_dict_add(struct hash_dict **this, const char *key, const char *value);
void hash_dict_del(struct hash_dict **this, const char *key);
const char *hash_dict_get(struct hash_dict *this, const char *key);
void hash_dict_print(struct hash_dict *this);

#endif
