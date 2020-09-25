#include <stdio.h>
#include "hash_dict.h"

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
