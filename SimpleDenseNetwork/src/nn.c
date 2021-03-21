#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/nn.h"

float MSE(float yhat, float y)
{
    float mse = pow((y - yhat), 2.0);
    return mse;
}

void makeWeights(int rows, int cols, float *x)
{
    srand(time(NULL));
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            x[i*rows+j] = (((float)rand()/(float)(RAND_MAX)));
        }
    }
}

void printNetwork(layer *hiddenLayers[], char *argv[])
{
    printf("Layer 0:\nInput %d\n", COLS);
    for(int i = 0, i < atoi(argv[1]); i++)
    {
        printf("Layer %d:\nInput %d\tOutput %d\n");
    }
    printf("Output Layer:\nOutput %d", OUTPUT);
}
