// Elapsed Real Time for basis-5.txt:
// real    0m2.994s
// user    0m0.939s
// sys     0m1.916s
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <cuda_runtime.h>

// General function to report a failure and exit.
static void fail(char const *message)
{
  fprintf(stderr, "%s\n", message);
  exit(1);
}

// Print out a usage message, then exit.
static void usage()
{
  printf("usage: shortest [report]\n");
  exit(1);
}

// Maximum width of each basis vector.
#define DMAX 100

// Type that can be used to represent a basis vector.
typedef int BVec[DMAX];

// Actual length of each vector in the basis.
int width;

// List of all vectors defining the lattice.
BVec *basis;

// Number of vectors representing the basis.
int bCount = 0;

// Current capacity of the basis array
int bCap = 0;

// Read the input list of basis vectors.
void readBasis()
{
  // Determine the size of the basis
  if (scanf("%d", &width) != 1)
    fail("Can't read vector length");

  // Set up initial list and capacity.
  bCap = 5;
  basis = (BVec *)malloc(bCap * sizeof(BVec));

  // Read all the basis vectors from the input.
  int v;
  while (scanf("%d", &v) == 1)
  {
    // Grow the basis array if needed.
    if (bCount >= bCap)
    {
      bCap *= 2;
      basis = (BVec *)realloc(basis, bCap * sizeof(BVec));
    }

    // Fill in the next basis vector.
    basis[bCount][0] = v;
    for (int i = 1; i < width; i++)
      if (scanf("%d", &(basis[bCount][i])) != 1)
        fail("Can't read vector element");
    bCount += 1;
  }
}

__global__ void findShortest(BVec *devBasis, int *devResults, int bCount, int width, bool report)
{
  // Unique index for this worker.
  int i = blockDim.x * blockIdx.x + threadIdx.x;
  int j = blockDim.y * blockIdx.y + threadIdx.y;

  if (i < bCount - 2 && j < width)
  {
    devResults[i] = 1000000000;
    // int shortestSum = 1000000000; // Initialize with a large value
    // Compute the locally shortest squared magnitude
    // Second Vector
    for (int k = i + 1; k < bCount - 1; k++)
    {
      // Third Vector
      for (int l = k + 1; l < bCount; l++)
      {
        int sum = 0;
        // Columns
        for (int m = 0; m < width; m++)
        {
          int sqrVal = (devBasis[i][j + m] + devBasis[k][j + m] + devBasis[l][j + m]);
          sum += (sqrVal * sqrVal);
        }
        // Check if local smaller
        if (sum < devResults[i])
        {
          devResults[i] = sum;
          // printf("i: %d shortestSum: %d\n", i, devResults[i]);
        }
        if (report == true)
        {
          printf("%d : | B[%d] + B[%d] + B[%d] | = %d\n", i, i, k, l, sum);
        }
      }
    }

    // Store the result in devResults
    // devResults[i] = shortestSum;
  }
  // for (int g = 0; g < sizeof(devResults); g++)
  // {
  //   printf("%d ", devResults[g]);
  // }
  // printf("\n");
}

int main(int argc, char *argv[])
{
  // If there's an argument, it better be "report"
  bool report = false;
  if (argc == 2)
  {
    if (strcmp(argv[1], "report") != 0)
      usage();
    report = true;
  }

  readBasis();

  // Allocate space on the device to hold a copy of the basis.
  BVec *devBasis = NULL;
  if (cudaMalloc((void **)&devBasis, (bCount) * sizeof(BVec)) != cudaSuccess)
  {
    fail("Failed to allocate space for lenght list on device");
  }
  // Copy the list over to the device.
  cudaMemcpy(devBasis, basis, (bCount) * sizeof(BVec), cudaMemcpyHostToDevice);
  // Allocate space on the device to hold the results.
  int *devResults = NULL;
  // long ncr = (bcount) * (bcount - 1) * (bcount - 2) / 6;
  if (cudaMalloc((void **)&devResults, (bCount - 2) * sizeof(int)) != cudaSuccess)
  {
    fail("Failed to allocate space for lenght list on device");
  }
  // Block and grid dimensions.
  int threadsPerBlock = 100;
  // Round up.
  int blocksPerGrid = (bCount + threadsPerBlock - 1) / threadsPerBlock;
  // Run our kernel on these block/grid dimensions
  findShortest<<<blocksPerGrid, threadsPerBlock>>>(devBasis, devResults, bCount, width, report);
  if (cudaGetLastError() != cudaSuccess)
    fail("Failure in CUDA kernel execution.");

  // Add code to copy the results list back to the host.
  int *results = (int *)malloc((bCount - 2) * sizeof(int));
  // printf("1: %d\n", sizeof(results));
  cudaMemcpy(results, devResults, (bCount - 2) * sizeof(int), cudaMemcpyDeviceToHost);
  // Find the minimum of all the results and report it.
  int small = results[0];
  // printf("2: %d , bCount: %d , sizeof(int): %d\n", sizeof(results), bCount, sizeof(int *));
  for (int i = 1; i < (bCount - 2); i++)
  {
    // printf("%d ", results[i]);
    if (results[i] < small && results[i] != 0)
    {
      small = results[i];
    }
  }
  printf("Shortest Vector: %.3f\n", sqrt(small));
  // Free memory on the device and the host.
  cudaFree(devBasis);
  cudaFree(devResults);
  free(results);
  cudaDeviceReset();

  return 0;
}