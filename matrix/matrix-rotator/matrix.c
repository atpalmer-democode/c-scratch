#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

struct matrix {
    int **matrix;
    int size;
};

static void array_rotate_forward(int *points[], int count) {
    int temp = *points[count - 1];
    for(int i = count - 1; i > 0; --i) {
        *points[i] = *points[i - 1];
    }
    *points[0] = temp;
}

static void array_rotate_reverse(int *points[], int count) {
    int temp = *points[0];
    for(int i = 0; i < count - 1; ++i) {
        *points[i] = *points[i + 1];
    }
    *points[count - 1] = temp;
}

const matrix_t *matrix_new(int size) {
    if(size < 0) {
        return NULL;
    }

    matrix_t *this = malloc(sizeof(matrix_t));
    this->size = size;

    this->matrix = malloc(sizeof(int *) * this->size);
    for(int row = 0; row < size; ++row) {
        this->matrix[row] = malloc(sizeof(int) * this->size);
        for(int column = 0; column < size; ++column) {
            this->matrix[row][column] = (row + 1) * (column + 1);
        }
    }

    return this;
}

void matrix_free(const matrix_t *this) {
    assert(this != NULL);
    for(int row = 0; row < this->size; ++row) {
        free(this->matrix[row]);
    }
    free(this->matrix);
    free((void *)this);
}

void matrix_rotate_with_function(const matrix_t *this, void (*rotator)(int *points[], int count)) {
    assert(this != NULL && rotator != NULL);
    for(int orbit = 0; orbit < this->size / 2; ++orbit) {
        int left_column = orbit;
        int right_column = this->size - 1 - orbit;
        int top_row = orbit;
        int bottom_row = this->size - 1 - orbit;

        for(int i = 0; i < this->size - 1 - orbit * 2; ++i) {
            int *points[] = {
                &this->matrix[top_row][left_column + i],
                &this->matrix[top_row + i][right_column],
                &this->matrix[bottom_row][right_column - i],
                &this->matrix[bottom_row - i][left_column],
            };

            rotator(points, sizeof(points) / sizeof(*points));
        }
    }
}

void matrix_rotate(const matrix_t *this) {
    matrix_rotate_with_function(this, &array_rotate_forward);
}

void matrix_rotate_reverse(const matrix_t *this) {
    matrix_rotate_with_function(this, &array_rotate_reverse);
}

void matrix_print(const matrix_t *this) {
    assert(this != NULL);
    for(int row = 0; row < this->size; ++row) {
        for(int column = 0; column < this->size; ++column) {
            printf(" %3d ", this->matrix[row][column]);
        }
        printf("\n");
    }
    printf("\n");
}
