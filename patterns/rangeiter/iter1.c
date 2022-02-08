#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct iter_state {
    int curr;
    int stop;
    int step;
};

struct iter_state *iter_range(int start, int stop, int step) {
    assert(step != 0);
    struct iter_state *new = malloc(sizeof *new);
    new->curr = start - step;
    new->stop = stop;
    new->step = step;
    return new;
}

void iter_free(struct iter_state *self) {
    free(self);
}

int iter_has_next(struct iter_state *self) {
    if(self->step < 0)
        return self->curr + self->step > self->stop;
    if(self->step > 0)
        return self->curr + self->step < self->stop;
    assert(0);
}

int iter_next(struct iter_state *self) {
    self->curr += self->step;
    return self->curr;
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
