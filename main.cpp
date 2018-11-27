// Andrew Gonzalez
// CS 350
// The intent of this project is to measure runtime performance for
// the Quicksort algorithm using various pivot choosing strategies.

#include <iostream>
#include <chrono>
#include <algorithm>
#include "quicksort.h"

using namespace std::chrono;

typedef int (*partFunc)(int*,int,int,int);
typedef int (*pivotFunc)(int*,int,int);

// prototypes
microseconds startSort(int array[], int arrSize,
        int (*partitionFunc)(int*,int,int,int), int (*pivotFunc)(int*,int,int));
partFunc determinePartition(char*);
pivotFunc determinePivot(char*);
void printArr(int *pInt, int arrSize);

int main(int argc, char *argv[]) {
    int (*partitionFunc)(int*,int,int,int);
    int (*pivotFunc)(int*,int,int);
    int * sourceArray;
    int * copyArray;
    int arrSize = 100;
    // Using a Mersenne twister as the engine to generate random numbers.
    // This will generate uints, and I need ints, so I'm using the
    // uniform_int_distribution engine to transform the uints
    // into ints within the range 0...INT32_MAX.
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(0, INT32_MAX);
    // chrono::duration class template using microseconds
    microseconds timeTaken;

    if (3 == argc) {
        partitionFunc = determinePartition(argv[1]);
        pivotFunc = determinePivot(argv[2]);
    }
    else {
        partitionFunc = lomutoPartition;
        pivotFunc = basicPivot;
    }

    while (arrSize <= 10000) {
        sourceArray = new int[arrSize];
        copyArray = new int[arrSize];
        // Fill source array with random numbers
        for (int i = 0; i < arrSize; ++i) {
            sourceArray[i] = dist(randGen);
        }

        std::cout << "\n\nStarting sort..." << std::endl;
        for (int i = 0; i < 50; ++i) {
            std::copy(sourceArray, sourceArray + arrSize, copyArray);
            timeTaken += startSort(copyArray, arrSize, partitionFunc, pivotFunc);
        }
        timeTaken /= 50; // the average of 50 runs of quicksort
        std::cout << "Finished (size: " << arrSize << "): " << timeTaken.count() << " microseconds" << std::endl;
        delete[] copyArray;
        delete[] sourceArray;
        arrSize += 100;
    }
    return 0;
}


// Run quicksort using the pivot and partition functions given.
microseconds startSort(int array[], int arrSize,
        int (*partitionFunc)(int*,int,int,int), int (*pivotFunc)(int*,int,int)) {
    high_resolution_clock clock;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    microseconds timeTaken;

    start = clock.now();
    quicksort(array, 0, arrSize - 1, partitionFunc, pivotFunc);
    end = clock.now();
    timeTaken += duration_cast<microseconds>(end - start);

    return timeTaken;
}


partFunc determinePartition(char * partition) {
    if (strcmp("lomuto", partition) == 0)
        return lomutoPartition;
    else if (strcmp("hoare", partition) == 0)
        return hoarePartition;
    // default to hoare partition if no match
    return hoarePartition;
}


pivotFunc determinePivot(char * pivot) {
    if (strcmp("basic", pivot) == 0)
        return basicPivot;
    else if (strcmp("random", pivot) == 0)
        return randomPivot;
    else if (strcmp("m3", pivot) == 0)
        return medianOfThree;
    else if (strcmp("mM", pivot) == 0)
        return medianOfMedians;
    // default to basic pivot if no match
    return basicPivot;
}


void printArr(int *pInt, int arrSize) {
    for (int i = 0; i < arrSize; ++i)
        std::cout << pInt[i] << std::endl;
}

