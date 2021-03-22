//
// Created by alex on 3/21/21.
//


#ifndef LINEAR_LINEAR_H
#define LINEAR_LINEAR_H

typedef struct linearLayer{
    float *weights;
    int *derivative;
    float *output;
    int in;
    int out;
    void (*acvitation_function)(struct linearLayer*);
    void (*free_layer)(struct linearLayer*);
};

static void freeLayer(struct linearLayer*);

void initLinear(struct linearLayer* layer, int rows, int cols, struct activation *funcs);

#endif //LINEAR_LINEAR_H