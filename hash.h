#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <string.h>

long bytes_hash(uint8_t *key, size_t len);

#define STRING_HASH(key)            (bytes_hash((void *)(key), strlen(key)))

#endif
