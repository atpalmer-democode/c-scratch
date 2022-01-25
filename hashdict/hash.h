#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <string.h>

uint64_t bytes_hash(uint8_t *key, ssize_t len);

#define STRING_HASH(key)            (bytes_hash((void *)(key), strlen(key)))

#endif
