#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    char c;
} Item;

typedef struct {
    int count;
    Item *items[];
} Items;

#define TO_KEY(i) (((i) % 26))
#define TO_ALPHA(i) (((i) % 26) + 65)

Item *Item_rand(void) {
    Item *item = malloc(sizeof *item);
    int val = rand();
    item->key = TO_KEY(val);
    item->c = TO_ALPHA(val);
    return item;
}

void Item_free(Item *this) {
    free(this);
}

Item *Item_copy(Item *this) {
    Item *new = malloc(sizeof *new);
    new->key = this->key;
    new->c = this->c;
    return new;
}

#define ITEMS_ALLOC(count) (malloc(sizeof(Items) + count * sizeof(Item *)))

Items *Items_new(void) {
    static const int COUNT = 15;
    Items *new = ITEMS_ALLOC(COUNT);
    new->count = 0;
    for(int i = 0; i < COUNT; ++i) {
        new->items[i] = Item_rand();
        ++new->count;
    }
    return new;
}

void Items_free(Items *this) {
    for(int i = 0; i < this->count; ++i)
        Item_free(this->items[i]);
    free(this);
}

Items *Items_copy(Items *this) {
    Items *new = ITEMS_ALLOC(this->count);
    new->count = 0;
    for(int i = 0; i < this->count; ++i) {
        new->items[i] = Item_copy(this->items[i]);
        ++new->count;
    }
    return new;
}

void Items_print(Items *this) {
    printf("Items count: %d\n", this->count);
    for(int i = 0; i < this->count; ++i)
        printf("%c (%d); ", this->items[i]->c, this->items[i]->key);
    printf("\n");
}

void Items_selection_sort(Items *this) {
    for(int place = 0; place < this->count - 1; ++place) {
        for(int seek = place + 1; seek < this->count; ++seek) {
            if(this->items[seek]->key < this->items[place]->key) {
                Item *tmp = this->items[place];
                this->items[place] = this->items[seek];
                this->items[seek] = tmp;
            }
        }
    }
}

void Items_insertion_sort(Items *this) {
    for(int curr = 1; curr < this->count; ++curr) {
        Item *item = this->items[curr];
        int seek = curr - 1;
        while(seek > -1 && item->key < this->items[seek]->key) {
            this->items[seek + 1] = this->items[seek];
            --seek;
        }
        this->items[seek + 1] = item;
    }
}

void run(Items *_items, void (*sort)(Items *)) {
    Items *items = Items_copy(_items);
    Items_print(items);
    sort(items);
    Items_print(items);
    Items_free(items);
}

int main(void) {
    Items *items = Items_new();
    run(items, Items_selection_sort);
    run(items, Items_insertion_sort);
    Items_free(items);
}
