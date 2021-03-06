#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tensor.h"

static void reallocateMem(Matrix *matrix, struct Shape shape)
{
    //matrix->shape.x = x;
    //matrix->shape.y = y;
    matrix->data = realloc(matrix->data, sizeof(float)*matrix->shape.x*matrix->shape.y);
}

static void freeMatrix(Matrix *matrix)
{
    free(matrix->data);
    free(matrix);
}

static void input(Matrix* matrix, float* inMatrix)
{
    memcpy( matrix->data, inMatrix, sizeof(float)*matrix->shape.x*matrix->shape.y);
}

Matrix* createMatrix( struct Shape shape)
{
    Matrix* matrix = malloc(sizeof(Matrix));
    matrix->shape.x = shape.x;
    matrix->shape.y = shape.y;
    matrix->giveMem = reallocateMem;
    matrix->freeMem = freeMatrix;
    matrix->inputData = input;
    matrix->data = malloc(sizeof(float)*matrix->shape.x*matrix->shape.y);
    return matrix;
}

