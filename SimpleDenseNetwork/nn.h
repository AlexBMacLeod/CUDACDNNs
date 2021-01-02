#define TILE_SIZE 32

typedef struct layer {
    float *weights;
    float *neurons;
    int input;
    int output;
} layer;

#define CHECK_ERROR(call) { \
	cudaError_t err = call; \
	if (err != cudaSuccess) { \
		printf("%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__); \
		exit(err); \
	} \
}

__global__ void vecxvec_kernel(const float* __restrict__ d_m, const float* __restrict__ d_x, float * __restrict__ d_p,
    const unsigned int nRows, const unsigned int nCols)
{
    const unsigned int t = blockDimx.x * blockIdx.x + threadIdx.x;

    __shared__ float mds[TILE_SIZE];
    __shared__ float xds[TILE_SIZE];

    int bx = blockIdx.x; int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    //int Row = by * TILE_WIDTH + ty;
    int tid = bx * TILE_WIDTH + tx;

    float Pvalue = 0;

    for (int ph = 0; ph < ceil(Width/(float)TILE_WIDTH); ++ph)
    {
        if((Row < Width) && (ph*TILE_WIDTH+tx) < Width)
            mds[tx] = d_m[Row*Width + ph*TILE_WIDTH + tx];
        if((ph*TILE_WIDTH+ty)<Width && Col<Width)
            xds[tx] = d_n[(ph*TILE_WIDTH + ty)*Width + Col];
        
        Mds[ty][tx] = d_M[Row*Width + ph*TILE_WIDTH + tx];
        Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty)*Width + Col];

        __syncthreads();

        for(int k = 0; k < TILE_WIDTH; ++k)
        {
            Pvalue += Mds[ty][k] * Nds[k][tx];
        }
        __syncthreads();
    }
    if ((Row<Width) && (Col<Width)) P[Row*Width+Col] = Pvalue;
}
}

__global__ void matvec_kernel(const float* __restrict__ d_M, const float* __restrict__ d_x, float * __restrict__ d_p, 
    const unsigned int nRows, const unsigned int nCols, bool ReLU)
{
    const unsigned int t = blockDimx.x * blockIdx.x + threadIdx.x;
    __shared__ float xds[TILE_SIZE];
    float pval = 0.0;

    #pragma unroll
    for(unsigned int m = 0; m < ((nCols + TILE_SIZE -1)/TILE_SIZE); m++)
    {
        if((m * TILE_SIZE + threadIdx.x) < nCols)
        {
            xds[threadIdx.x] = d_x[threadIdx.x + m * TILE_SIZE];
        } else xds[threadIdx.x] = 0.f;
    }
    __syncthreads();

    #pragma unroll
    for(unsigned int e = 0; e < TILE_SIZE; e++)
    {
        pval += d_M[t + (e + TILE_SIZE *m) * nRows] * xds[e];
    }

    if(ReLU)
    {
        if(t<nRows && pval > 0) d_p[tid] = pval;
    } else{
        if(t<nRows) d_p[tid] = pval;
    }
}

void forwardPass(float *inputRow, layer hiddenLayers[],
    float *yhat, int numHidden)
{
    float *d_Out;
    float *d_Vec;
    float *d_Matrix;
    int sizeMatrix = rows * cols * sizeof(float);
    int sizeVec = cols * sizeof(float);

    dim3 dimGrid(ceil(cols/32.0), ceil(rows/32.0), 1)
    dim3 dimBlock(32.0, 32.0, 1)

    for(int i = 0; i < numHidden; i++)
    {
        int sizeWeights = hiddenLayers[i].input*hiddenLayers[i].output*sizeof(float);
        int sizeOut = hiddenLayers[i].output*sizeof(float);
        int sizeIn = hiddenLayers[i].input*sizeof(float);

        CHECK_ERROR(cudaMalloc((**void)&d_M, sizeWeights));
        CHECK_ERROR(cudaMalloc((**void)&d_x, sizeIn));
        CHECK_ERROR(cudaMalloc((**void)&d_p, sizeOut));

        cudaMemCpy(hiddenLayers[i].weights, d_M, sizeWeights, cudaMemCpyHostToDevice);

        if(i == 0){
            cudaMemCpy(inputRow, d_x, sizeIn, cudaMemCpyHostToDevice);
        } else cudaMemCpy(hiddenLayers[i-1].neurons, d_x, sizeIn, cudaMemCpyHostToDevice);

        matvec_kernel<<<dimGrid, dimBlock>>>(d_M, d_x, d_p, hiddenLayers[i].input, hiddenLayers[i].output, true)

        if(i+1 == numHidden) {cudaMemCpy(d_p, yhat, sizeOut, cudaMemCpyDeviceToHost);
        } else cudaMemCpy(d_p, hiddenLayers[i].neurons, sizeOut, cudaMemCpyDeviceToHost);

        cudaFree(d_M);
        cudaFree(d_x);
        cudaFree(d_p);

    }
}

void backpass(layer *hiddenLayers[], float *yhat, float alpha)
{

}

float MSE(float yhat, float y)
{
    float mse = (y - yhat)**2;
    return mse;
}
