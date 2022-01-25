#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct iter_state {
    int start;
    int stop;
    int step;
    int next_pos;
};

#define ITER_NEXT(self)     ((self)->start + (self)->next_pos * (self)->step)

struct iter_state *iter_range(int start, int stop, int step) {
    assert(step != 0);
    struct iter_state *new = malloc(sizeof *new);
    new->start = start;
    new->stop = stop;
    new->step = step;
    new->next_pos = 0;
    return new;
}

void iter_free(struct iter_state *self) {
    free(self);
}

int iter_has_next(struct iter_state *self) {
    int mult = (self->step < 0) ? -1 : 1;
    return mult * ITER_NEXT(self) < mult * self->stop;
}

int iter_next(struct iter_state *self) {
    int result = ITER_NEXT(self);
    ++self->next_pos;
    return result;
}

void test_pos(void) {
    printf("test_pos:\n");
    struct iter_state *iter = iter_range(1, 10, 1);
    while(iter_has_next(iter))
        printf("value: %d\n", iter_next(iter));
    iter_free(iter);
}

void test_neg(void) {
    printf("test_neg:\n");
    struct iter_state *iter = iter_range(10, 1, -1);
    while(iter_has_next(iter))
        printf("value: %d\n", iter_next(iter));
    iter_free(iter);
}

int main(void) {
    test_pos();
    test_neg();
}
