#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hash_item {
    const char *key;
    const char *value;
};

struct hash_dict {
    size_t count;
    size_t buckets;
    struct hash_item items[];
};

static long string_hash(const char *key) {
    /* djbx33a hash */
    long result = 5381;
    for(size_t i = 0; i < strlen(key); ++i) {
        result = (result << 5) + result + key[i];
    }
    return result;
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

#define HASH_BUCKET(this, key)  (string_hash(key) % (this)->buckets)

static size_t _next_hash_bucket(struct hash_dict *this, const char *key) {
    size_t bucket = HASH_BUCKET(this, key);
    while(this->items[bucket].key) {
        ++bucket;
        if(bucket > this->buckets)
            bucket = 0;
    }
    return bucket;
}

static void _hash_dict_add(struct hash_dict *this, const char *key, const char *value) {
    size_t bucket = _next_hash_bucket(this, key);
    this->items[bucket].key = key;
    this->items[bucket].value = value;
    ++this->count;
}

void hash_dict_rehash(struct hash_dict **this) {
    size_t new_count = ((*this)->count + 1) * 2;
    if((*this)->buckets > new_count)
        return;
    struct hash_dict *new = hash_dict_new(new_count);
    for(size_t i = 0; i < (*this)->buckets; ++i) {
        struct hash_item *item = &(*this)->items[i];
        if(!item->key)
            continue;
        _hash_dict_add(new, item->key, item->value);
    }
    hash_dict_free(*this);
    *this = new;
}

void hash_dict_add(struct hash_dict **this, const char *key, const char *value) {
    hash_dict_rehash(&*this);
    _hash_dict_add(*this, key, value);
}

const char *hash_dict_get(struct hash_dict *this, const char *key) {
    size_t bucket = HASH_BUCKET(this, key);
    while(this->items[bucket].key) {
        if(strcmp(this->items[bucket].key, key) == 0)
            return this->items[bucket].value;
        ++bucket;
        if(bucket > this->buckets)
            bucket = 0;
    }
    return NULL;
}

void hash_dict_print(struct hash_dict *this) {
    printf("{\n");
    for(size_t i = 0; i < this->buckets; ++i) {
        struct hash_item *item = &this->items[i];
        if(!item->key) {
            continue;
        }
        printf("  %lu.) %s: %s\n", i, item->key, item->value);
    }
    printf("}\n");
}

int main(void) {
    struct hash_dict *dict = hash_dict_new(1);

    hash_dict_add(&dict, "fname", "Andrew");
    hash_dict_add(&dict, "lname", "Palmer");
    hash_dict_add(&dict, "mname", "T.");

    const char *fname = hash_dict_get(dict, "fname");
    const char *mname = hash_dict_get(dict, "mname");
    const char *lname = hash_dict_get(dict, "lname");

    printf("Get Results: %s %s %s\n", fname, mname, lname);
    hash_dict_print(dict);
    hash_dict_free(dict);
}
