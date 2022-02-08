#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
} Item;

typedef struct {
    int count;
    Item *items[];
} ArrayTree;

ArrayTree *ArrayTree_new(void) {
    static const int COUNT = 32;
    ArrayTree *new = malloc(sizeof(ArrayTree) + COUNT * sizeof(Item *));
    new->count = 0;
    for(int i = 0; i < COUNT; ++i) {
        Item *item = malloc(sizeof(Item));
        item->value = rand() % 100;
        new->items[i] = item;
        ++new->count;
    }
    return new;
}

void ArrayTree_free(ArrayTree *this) {
    for(int i = 0; i < this->count; ++i) {
        free(this->items[i]);
    }
    free(this);
}

#define LEVEL_START(level)  ((1 << (level)) - 1)
#define LEVEL_COUNT(level)  (1 << (level))

void ArrayTree_print(ArrayTree *this) {
    for(int level = 0;;++level) {
        for(int li = 0; li < LEVEL_COUNT(level); ++li) {
            int i = LEVEL_START(level) + li;
            if(i >= this->count) {
                printf("\n");
                return;
            }
            printf("%d ", this->items[i]->value);
        }
        printf("\n");
    }
}

void ArrayTree_print_linear(ArrayTree *this) {
    for(int i = 0; i < this->count; ++i) {
        printf("%d ", this->items[i]->value);
    }
    printf("\n");
}

int main(void) {
    ArrayTree *tree = ArrayTree_new();
    printf("TREE VIEW:\n");
    ArrayTree_print(tree);
    printf("ARRAY VIEW:\n");
    ArrayTree_print_linear(tree);
    ArrayTree_free(tree);
}
