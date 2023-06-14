#ifndef FUNC_H
#define FUNC_H

typedef struct row{
    int size;
    int* next;
}row;

typedef struct matrix{
    int size;
    row* matr;
}matrix;

void destructor (matrix ** m);
int* create_line(int m, int i);
void matrix_create(matrix** full);
matrix* newmatrix(const matrix* full);

#endif
