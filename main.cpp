// Andrew Gonzalez
// CS 350
// The intent of this project is to measure runtime performance for
// the Quicksort algorithm using various pivot choosing strategies.

#include <iostream>
#include <chrono>
#include "quicksort.h"

int main() {
    using namespace std::chrono;
    int arrSize = 10000;
    high_resolution_clock clock;
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    milliseconds time;

    // Using a Mersenne twister as the engine to generate random numbers.
    // This will generate uints, and I need ints, so I'm using the
    // uniform_int_distribution engine to transform the uints
    // into ints within the range 0...INT32_MAX.
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(0, INT32_MAX);

    int * testArr = new int[arrSize];
    for (int i = 0; i < arrSize; ++i) {
        testArr[i] = dist(randGen);
        //std::cout << testArr[i] << std::endl;
    }
    std::cout << "\n\n\nStarting sort..." << std::endl;
    start = clock.now();
    quicksort(testArr, 0, arrSize-1, hoarePartition, randomPivot);
    end = clock.now();
    time = duration_cast<milliseconds>(end - start);
    std::cout << "Finished in: " << time.count() << " milliseconds" << std::endl;
    for (int i = 0; i < arrSize; ++i) {
        std::cout << testArr[i] << std::endl;
    }
    delete [] testArr;
    return 0;
}