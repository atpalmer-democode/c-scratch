#include "hash.h"

uint64_t bytes_hash(uint8_t *key, ssize_t len) {
    /* djbx33a hash */
    uint64_t result = 5381;
    for(ssize_t i = 0; i < len; ++i) {
        result = (result << 5) + result + key[i];
    }
    return result;
}
