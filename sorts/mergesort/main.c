#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arr {
    int count;
    int items[];
};

struct arr *arr_create(int count, int mod) {
    struct arr *new = malloc(sizeof(struct arr) + count * sizeof(int));
    new->count = count;
    for(int i = 0; i < count; ++i)
        new->items[i] = rand() % mod;
    return new;
}

void arr_free(struct arr *this) {
    free(this);
}

void arr_print(struct arr * this) {
    printf("[ ");
    for(int i = 0; i < this->count; ++i) {
        printf("%d ", this->items[i]);
    }
    printf("]\n");
}

struct arr *_arr_slice(struct arr *this, int start, int end) {
    int count = end - start + 1;
    struct arr *new = malloc(sizeof *new + count * sizeof(int));
    new->count = count;
    memcpy(new->items, &this->items[start], count * sizeof(int));
    return new;
}

struct arr *arr_sort(struct arr *this) {
    if(this->count < 2)
        return this;

    int mid = (this->count / 2) - 1;

    struct arr *left = arr_sort(_arr_slice(this, 0, mid));
    struct arr *right = arr_sort(_arr_slice(this, mid + 1, this->count - 1));

    int i = 0;
    int li = 0, ri = 0;

    while(li < left->count && ri < right->count) {
        if(left->items[li] < right->items[ri])
            this->items[i++] = left->items[li++];
        else
            this->items[i++] = right->items[ri++];
    }

    while(li < left->count)
        this->items[i++] = left->items[li++];
    while(ri < right->count)
        this->items[i++] = right->items[ri++];

    arr_free(left);
    arr_free(right);

    return this;
}

int main(void) {
    struct arr *items = arr_create(10, 100);
    arr_print(items);
    arr_sort(items);
    arr_print(items);
    arr_free(items);
}
