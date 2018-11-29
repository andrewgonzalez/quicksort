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
bool checkCorrectness(int *checkArray, int arrSize);
void printArr(int *pInt, int arrSize);


int main(int argc, char *argv[]) {
    int (*partitionFunc)(int*,int,int,int);
    int (*pivotFunc)(int*,int,int);
    void (*fillArray)(int*,int);
    int * sourceArray;
    // beginning array size of 100 and max of 1 million
    int arrSize = 1000;
    int maxSize = 1000000;
    int loopCount = 20;
    char filename[35];
    bool verifySort = false;

    high_resolution_clock clock;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    // chrono::duration class template using microseconds
    microseconds timeTaken;
    std::ofstream fout;

    // Use command line arguments to determine which functions to use
    if (4 == argc) {
        partitionFunc = determinePartition(argv[1], filename);
        pivotFunc = determinePivot(argv[2], filename);
        fillArray = determineArrayOrder(argv[3], filename);
    }
    else if (5 == argc) {
        partitionFunc = determinePartition(argv[1], filename);
        pivotFunc = determinePivot(argv[2], filename);
        fillArray = determineArrayOrder(argv[3], filename);
        if (strcmp("verify", argv[4]) == 0) {
            verifySort = true;
        }
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

    // If we're going to verify that the sort works correctly then we don't need
    // to sort a bunch of huge arrays, just one array will do.
    if (verifySort) {
        maxSize = 1000;
        loopCount = 1;
    }

    while (arrSize <= maxSize) {
        sourceArray = new int[arrSize];
        timeTaken = microseconds::zero();

        for (int i = 0; i < loopCount; ++i) {
            fillArray(sourceArray, arrSize);
            start = clock.now();
            quicksort(sourceArray, 0, arrSize - 1, partitionFunc, pivotFunc);
            end = clock.now();
            timeTaken += duration_cast<microseconds>(end - start);
        }
        timeTaken /= loopCount; // average the runs of quicksort
        fout << arrSize << ", " << timeTaken.count() << "\n";

        if (verifySort) {
            if (checkCorrectness(sourceArray, arrSize))
                std::cout << argv[1] << " " << argv[2] << " " << argv[3] << " sorts correctly" << std::endl;
            else
                std::cout << argv[1] << " " << argv[2] << " " << argv[3] << " does not sort correctly" << std::endl;
        }

        delete[] sourceArray;
        arrSize += 1000;
    }
    fout.close();
    return 0;
}


// Determine which partition algorithm the user entered at the command line.
// Input: two c-style strings (char array), one for the partition to use for quicksort,
// and a string to use for the filename respectively.
// Output: a function pointer to the partition function to use.
partFunc determinePartition(char * partition, char * filename) {
    if (strcmp("lomuto", partition) == 0) {
        strcat(filename, "lomuto");
        return lomutoPartition;
    }
    // default to hoare partition if no match
    strcat(filename, "hoare");
    return hoarePartition;
}


// Determine which pivot algorithm the user entered at the command line.
// Input: two c-style strings (char array), one for the pivot to use for quicksort,
// and a string to use for the filename respectively.
// Output: a function pointer to the pivot function to use.
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


// Determine which array filling algorithm the user entered at the command line.
// Input: two c-style strings (char array), one for the array fill to use for quicksort,
// and a string to use for the filename respectively.
// Output: a function pointer to the array filling function to use.
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


// Fill the passed in int array with random integers.
// Input: an int array and an int for the array size.
// Output: void
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


// Fill the passed in array with mostly sorted integers. Every 10th element
// will have a random integer.
// Input: an int array and an int for the array size.
// Output: void
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


// Fill the passed in array with sorted integers (ascending order).
// Input: an int array and an int for the array size.
// Output: void
void sortedOrder(int *sourceArray, int arrSize) {
    // Fill source array with sorted data
    for (int i = 0; i < arrSize; ++i) {
        sourceArray[i] = i;
    }
}


// Fill the passed in array with reverse sorted integers (descending order).
// Input: an int array and an int for the array size.
// Output: void
void reverseOrder(int *sourceArray, int arrSize) {
    int j;
    for (int i = 0; i < arrSize; ++i) {
        j = arrSize - i;
        sourceArray[i] = j;
    }
}


// Checks if an array is sorted by comparing each element to the next. If the
// current element is greater than the next, then the array is not sorted.
// Input: an int array and an int for the array size.
// Output: true if the array is sorted, false otherwise.
bool checkCorrectness(int *checkArray, int arrSize) {
    bool isCorrect = true;
    for (int i = 0; isCorrect && i < arrSize - 1; ++i) {
        if (checkArray[i] > checkArray[i+1])
            isCorrect = false;
    }
    return isCorrect;
}


// Print the contents of an array to the standard output.
// Input: an int array and an int for the array size.
// Output: void
void printArr(int *pInt, int arrSize) {
    for (int i = 0; i < arrSize; ++i)
        std::cout << pInt[i] << std::endl;
}
