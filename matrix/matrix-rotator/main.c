#include <assert.h>
#include <stdio.h>
#include "matrix.h"

int main(void) {
    const matrix_t *matrix = matrix_new(10);
    assert(matrix != NULL);
    matrix_print(matrix);
    matrix_rotate(matrix);
    matrix_print(matrix);
    matrix_rotate(matrix);
    matrix_rotate(matrix);
    matrix_print(matrix);
    matrix_rotate_reverse(matrix);
    matrix_print(matrix);
    matrix_rotate_reverse(matrix);
    matrix_rotate_reverse(matrix);
    matrix_print(matrix);
    matrix_free(matrix);
    return 0;
}
