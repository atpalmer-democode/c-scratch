#ifndef MATRIX_H

typedef struct matrix matrix_t;

const matrix_t *matrix_new(int size);
void matrix_free(const matrix_t *this);
void matrix_rotate(const matrix_t *this);
void matrix_rotate_reverse(const matrix_t *this);
void matrix_print(const matrix_t *this);

#endif
