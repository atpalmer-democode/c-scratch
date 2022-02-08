#include <stdio.h>
#include <stdlib.h>

struct door_state {
    const char *text;
    struct door_state *handle;
    struct door_state *lock;
};

#define LOCKED   (&door_states[0])
#define CLOSED   (&door_states[1])
#define OPEN     (&door_states[2])

static struct door_state door_states[] = {
    {"LOCKED",  NULL,   CLOSED},
    {"CLOSED",  OPEN,   LOCKED},
    {"OPEN",    CLOSED, NULL},
};

void door_lock(struct door_state **state) {
    struct door_state *next = (*state)->lock;
    if(next == NULL) {
        fprintf(stderr, "Cannot use Lock. Door is %s.\n", (*state)->text);
        return;
    }
    *state = next;
    printf("Lock used. Door is now %s.\n", (*state)->text);
}

void door_handle(struct door_state **state) {
    struct door_state *next = (*state)->handle;
    if(next == NULL) {
        fprintf(stderr, "Cannot use Handle. Door is %s.\n", (*state)->text);
        return;
    }
    *state = next;
    printf("Handle used. Door is now %s.\n", (*state)->text);
}

int main(void) {
    struct door_state *door = LOCKED;
    printf("Door is %s\n", door->text);
    door_handle(&door);
    door_lock(&door);
    door_lock(&door);
    door_lock(&door);
    door_handle(&door);
    door_lock(&door);
    door_handle(&door);
    door_lock(&door);
}
