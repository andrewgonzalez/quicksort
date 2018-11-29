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
typedef void (*fillArr)(int*,int);

// prototypes
partFunc determinePartition(char*, char*);
pivotFunc determinePivot(char*, char*);
fillArr determineArrayOrder(char*, char*);
void randomOrder(int *sourceArray, int arrSize);
void partialOrder(int *sourceArray, int arrSize);
void sortedOrder(int *sourceArray, int arrSize);
void reverseOrder(int *sourceArray, int arrSize);
void printArr(int *pInt, int arrSize);

int main(int argc, char *argv[]) {
    int (*partitionFunc)(int*,int,int,int);
    int (*pivotFunc)(int*,int,int);
    void (*fillArray)(int*,int);
    int * sourceArray;
    int * copyArray;
    int arrSize = 100;
    char filename[35];
    // chrono::duration class template using microseconds
    high_resolution_clock clock;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    microseconds timeTaken;
    std::ofstream fout;

    if (3 == argc) {
        partitionFunc = determinePartition(argv[1], filename);
        pivotFunc = determinePivot(argv[2], filename);
        fillArray = randomOrder;
        strcat(filename, "Random");
    }
    else if (4 == argc) {
        partitionFunc = determinePartition(argv[1], filename);
        pivotFunc = determinePivot(argv[2], filename);
        fillArray = determineArrayOrder(argv[3], filename);
    }
    else {
        // default arguments
        partitionFunc = lomutoPartition;
        pivotFunc = basicPivot;
        fillArray = randomOrder;
        strcat(filename, "lomuto");
        strcat(filename, "basic");
        strcat(filename, "Random");
    }

    // open a file to print output to
    strcat(filename, ".csv");
    fout.open(filename, std::ofstream::out);
    fout << "Array Size, Time (microseconds)" << "\n";

    while (arrSize <= 10000) {
        sourceArray = new int[arrSize];
        copyArray = new int[arrSize];
        timeTaken = microseconds::zero();
        fillArray(sourceArray, arrSize);

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


fillArr determineArrayOrder(char * fillOrder, char * filename) {
    if (strcmp("sorted", fillOrder) == 0) {
        strcat(filename, "Sorted");
        return sortedOrder;
    }
    else if (strcmp("partsort", fillOrder) == 0) {
        strcat(filename, "Partial");
        return partialOrder;
    }
    else if (strcmp("reverse", fillOrder) == 0) {
        strcat(filename, "Reverse");
        return reverseOrder;
    }
    // default to randomly filled array
    strcat(filename, "Random");
    return randomOrder;
}


void randomOrder(int *sourceArray, int arrSize) {
    // Using a Mersenne twister as the engine to generate random numbers.
    // This will generate uints, and I need ints, so I'm using the
    // uniform_int_distribution engine to transform the uints
    // into ints within the range 0...INT32_MAX.
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(0, INT32_MAX);
    // Fill source array with random numbers
    for (int i = 0; i < arrSize; ++i) {
        sourceArray[i] = dist(randGen);
    }
}


void partialOrder(int *sourceArray, int arrSize) {
    int insertRandom = 10;
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(0, INT32_MAX);
    for (int i = 0; i < arrSize; ++i) {
        if ((i % insertRandom) == 0)
            sourceArray[i] = dist(randGen);
        else
            sourceArray[i] = i;
    }
}


void sortedOrder(int *sourceArray, int arrSize) {
    // Fill source array with sorted data
    for (int i = 0; i < arrSize; ++i) {
        sourceArray[i] = i;
    }
}


void reverseOrder(int *sourceArray, int arrSize) {
    int j;
    for (int i = 0; i < arrSize; ++i) {
        j = arrSize - i;
        sourceArray[i] = j;
    }
}


void printArr(int *pInt, int arrSize) {
    for (int i = 0; i < arrSize; ++i)
        std::cout << pInt[i] << std::endl;
}
