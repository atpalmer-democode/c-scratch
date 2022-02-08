#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct vec {
    int size;
    int data[];
};

struct vec *vec_create(int size, int mod) {
    srand(time(NULL));
    struct vec *new = malloc(sizeof *new + size * sizeof(*new->data));
    new->size = size;
    for(int i = 0; i < size; ++i)
        new->data[i] = rand() % mod;
    return new;
}

void vec_free(struct vec *this) {
    free(this);
}

void vec_print(struct vec *this) {
    printf("[ ");
    for(int i = 0; i < this->size; ++i)
        printf("%d ", this->data[i]);
    printf("]\n");
}

void _swap(int data[], int a, int b) {
    int tmp = data[a];
    data[a] = data[b];
    data[b] = tmp;
}

void _sort(int data[], int start, int end) {
    if(start >= end)
        return;
    int target = start;
    for(int i = start; i <= end; ++i) {
        if(data[i] < data[end])
            _swap(data, target++, i);
    }
    _swap(data, target, end);
    _sort(data, start, target - 1);
    _sort(data, target + 1, end);
}

void vec_sort(struct vec *this) {
    return _sort(this->data, 0, this->size - 1);
}

int main(void) {
    struct vec *vec = vec_create(20, 100);
    vec_print(vec);
    vec_sort(vec);
    vec_print(vec);
    vec_free(vec);
}
