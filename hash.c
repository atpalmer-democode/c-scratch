#include "hash.h"

long bytes_hash(uint8_t *key, ssize_t len) {
    /* djbx33a hash */
    long result = 5381;
    for(ssize_t i = 0; i < len; ++i) {
        result = (result << 5) + result + key[i];
    }
    return result;
}
