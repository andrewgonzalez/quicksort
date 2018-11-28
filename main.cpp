// Andrew Gonzalez
// CS 350
// The intent of this project is to measure runtime performance for
// the Quicksort algorithm using various pivot choosing strategies.

#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "quicksort.h"

using namespace std::chrono;

typedef int (*partFunc)(int*,int,int,int);
typedef int (*pivotFunc)(int*,int,int);

// prototypes
partFunc determinePartition(char*, char*);
pivotFunc determinePivot(char*, char*);
void printArr(int *pInt, int arrSize);

int main(int argc, char *argv[]) {
    int (*partitionFunc)(int*,int,int,int);
    int (*pivotFunc)(int*,int,int);
    int * sourceArray;
    int * copyArray;
    int arrSize = 100;
    char filename[25];
    // Using a Mersenne twister as the engine to generate random numbers.
    // This will generate uints, and I need ints, so I'm using the
    // uniform_int_distribution engine to transform the uints
    // into ints within the range 0...INT32_MAX.
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(0, INT32_MAX);
    high_resolution_clock clock;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    // chrono::duration class template using microseconds
    microseconds timeTaken;
    std::ofstream fout;

    if (3 == argc) {
        partitionFunc = determinePartition(argv[1], filename);
        pivotFunc = determinePivot(argv[2], filename);
    }
    else {
        // default arguments
        partitionFunc = lomutoPartition;
        pivotFunc = basicPivot;
        strcat(filename, "lomuto");
        strcat(filename, "basic");
    }

    // open a file to print output to
    strcat(filename, ".csv");
    fout.open(filename, std::ofstream::out);
    fout << "Array Size, Time (microseconds)" << "\n";

    while (arrSize <= 10000) {
        sourceArray = new int[arrSize];
        copyArray = new int[arrSize];
        timeTaken = microseconds::zero();
        // Fill source array with random numbers
        for (int i = 0; i < arrSize; ++i) {
            sourceArray[i] = dist(randGen);
        }

        std::cout << "\n\nStarting sort..." << std::endl;
        for (int i = 0; i < 50; ++i) {
            std::copy(sourceArray, sourceArray + arrSize, copyArray);
            start = clock.now();
            quicksort(copyArray, 0, arrSize - 1, partitionFunc, pivotFunc);
            end = clock.now();
            timeTaken += duration_cast<microseconds>(end - start);
        }
        timeTaken /= 50; // the average of 50 runs of quicksort
        std::cout << "Finished (size: " << arrSize << "): " << timeTaken.count() << " microseconds" << std::endl;
        fout << arrSize << ", " << timeTaken.count() << "\n";
        delete[] copyArray;
        delete[] sourceArray;
        arrSize += 100;
    }
    fout.close();
    return 0;
}


partFunc determinePartition(char * partition, char * filename) {
    if (strcmp("lomuto", partition) == 0) {
        strcat(filename, "lomuto");
        return lomutoPartition;
    }
    // default to hoare partition if no match
    strcat(filename, "hoare");
    return hoarePartition;
}


pivotFunc determinePivot(char * pivot, char * filename) {
    if (strcmp("random", pivot) == 0) {
        strcat(filename, "random");
        return randomPivot;
    }
    else if (strcmp("m3", pivot) == 0) {
        strcat(filename, "m3");
        return medianOfThree;
    }
    else if (strcmp("mM", pivot) == 0) {
        strcat(filename, "mM");
        return medianOfMedians;
    }
    // default to basic pivot if no match
    strcat(filename, "basic");
    return basicPivot;
}


void printArr(int *pInt, int arrSize) {
    for (int i = 0; i < arrSize; ++i)
        std::cout << pInt[i] << std::endl;
}

