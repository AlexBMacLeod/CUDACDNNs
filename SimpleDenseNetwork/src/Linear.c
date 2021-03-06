//
// Created by alex on 3/21/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "../include/activation_functions.cuh"
//#include "layer.h"
#include "../include/Linear.h"
//#include "layer.h"
#include "../include/nn.h"
//#include "nnKernel.cuh"

#define Fn_apply(type, fn, ...) {                                   \
void *stopper_for_apply = (int[]){0};                               \
type **list_for_apply = (type*[]){__VA_ARGS__, stopper_for_apply};  \
for (int i=0; list_for_apply[i] != stopper_for_apply; i++)          \
fn(list_for_apply[i]);                                              \
}

#define free_all(...) apply_func(void, free, __VA_ARGS__);


static void freeLayer( linearLayer* layer)
{
    layer->weights->freeMem(layer->weights);
    //freeMatrix(layer->derivative);
    layer->output->freeMem(layer->output);
    layer->input->freeMem(layer->input);
    free(layer);
}

void initLinear( linearLayer **layer, int in, int out)
{
    *layer = malloc(sizeof(linearLayer));

    //layer->derivative = createMatrix( in, out);
    (*layer)->weights = createMatrix( in, out);
    (*layer)->output = createMatrix( out, 1);
    (*layer)->input = createMatrix( in, 1);

    (*layer)->in = in;
    (*layer)->out = out;
    makeWeights( (*layer)->weights);
    //layer->actFunc = funcs->func;
    //layer->derivFunc = funcs->deriv;
    (*layer)->forward_pass = forward;
    (*layer)->free_layer = freeLayer;
}

Matrix* forward( linearLayer *layer, Matrix *input)
{
    layer->input->inputData( layer->input, input->data) ;
    //matrixVector(layer, input);
    //if(layer->actFunc != NULL) layer->actFunc(layer);
    return layer->output;
}

/*def backward(self, front):
if self.activation:
    delta = front*self._relu2deriv(self.out)
    out = delta.dot(self.weights.T)
else:
    delta = self.out - front
    out = delta.dot(self.weights.T)
self.weights -= alpha * self.input.T.dot(delta)
return out
void backward(struct linearLayer* layer, float* front)
{
    float* delta;
    delta = malloc(sizeof(float)*layer->out);

    if(layer->actFunc != NULL)
    {
        relu_deriv(layer);
        matrixVector( layer->output, front, delta, layer->in, layer->out)
        realloc(front, sizeof(float)*layer->in);
        float* transpose_weights;
        transpose_weights = malloc(sizeof(float)*layer->out*layer->in);
        transpose( layer->weights, transpose_weights, layer->in, layer->out);
        matrixVector( delta, transpose_weights, front);
        free(transpose_weights);
    } else{
        *delta = layer->output - *front;
        float* transpose_weights;
        transpose_weights = malloc(sizeof(float)*layer->out*layer->in);
        transpose( layer->weights, transpose_weights, layer->in, layer->out);
        matrixVector( delta, transpose_weights, front);
        free(transpose_weights);

    }

}*/