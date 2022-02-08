#include <stdio.h>
#include <stdlib.h>

struct light_pos {
    const char *name;
    struct light_pos *on;
    struct light_pos *off;
};

#define LIGHT_OFF   (&light_pos[0])
#define LIGHT_ON    (&light_pos[1])

static struct light_pos light_pos[] = {
    {.name="Off", .on=LIGHT_ON,   .off=NULL},
    {.name="On",  .on=NULL,       .off=LIGHT_OFF},
};


struct light_pos *light_init(void) {
    return LIGHT_OFF;
}

void light_message(struct light_pos *this) {
    printf("Light is now %s.\n", this->name);
}

void light_on(struct light_pos **this) {
    if(!(*this)->on) {
        fprintf(stderr, "Cannot switch light on. Light is currently %s.\n", (*this)->name);
        return;
    }
    *this = (*this)->on;
    light_message(*this);
}

void light_off(struct light_pos **this) {
    if(!(*this)->off) {
        fprintf(stderr, "Cannot switch light off. Light is currently %s.\n", (*this)->name);
        return;
    }
    *this = (*this)->off;
    light_message(*this);
}

int main(void) {
    struct light_pos *light = light_init();

    light_message(light);

    light_on(&light);
    light_on(&light);
    light_off(&light);
    light_on(&light);
    light_off(&light);
    light_off(&light);
}
