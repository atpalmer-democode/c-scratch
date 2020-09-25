#ifndef HASH_DICT_H
#define HASH_DICT_H

struct hash_item {
    long hash;
    const char *key;
    const char *value;
};

struct hash_dict {
    size_t count;
    size_t buckets;
    struct hash_item items[];
};

#define HASH_BUCKET(this, hash)     ((hash) % (this)->buckets)
#define STRING_HASH(key)            (bytes_hash((void *)(key), strlen(key)))

struct hash_dict *hash_dict_new(size_t buckets);
void hash_dict_free(struct hash_dict *this);
void hash_dict_resize(struct hash_dict **this);
void hash_dict_add(struct hash_dict **this, const char *key, const char *value);
const char *hash_dict_get(struct hash_dict *this, const char *key);
void hash_dict_print(struct hash_dict *this);

#endif
