#include <stdio.h>

/**
 * Source: https://developer.nvidia.com/content/how-query-device-properties-and-handle-errors-cuda-cc
 * Documentation of cudaDeviceProp: http://docs.nvidia.com/cuda/cuda-runtime-api/index.html#structcudaDeviceProp
 *
 * Sample line to compile:
 *   nvcc -arch=sm_20 -o printDeviceInfo printDeviceInfo.cu
 */

int main() {
        int nDevices;

        cudaGetDeviceCount(&nDevices);
        for (int i = 0; i < nDevices; i++) {
                cudaDeviceProp prop;
                cudaGetDeviceProperties(&prop, i);
                printf("Device Number: %d\n", i);
                printf("  Device name: %s\n", prop.name);
                printf("  Memory Clock Rate (KHz): %d\n", prop.memoryClockRate);
                printf("  Memory Bus Width (bits): %d\n", prop.memoryBusWidth);
                printf("  Peak Memory Bandwidth (GB/s): %f\n\n", 2.0*prop.memoryClockRate*(prop.memoryBusWidth/8)/1.0e6);
        }
}
