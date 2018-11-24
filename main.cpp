// Andrew Gonzalez
// CS 350
// The intent of this project is to measure runtime performance for
// the Quicksort algorithm using various pivot choosing strategies.

#include <iostream>
#include <random>
#include "quicksort.h"

int main() {
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(1, 100);

    int * testArr = new int[50];
    for (int i = 0; i < 50; ++i) {
        testArr[i] = dist(randGen);
        std::cout << testArr[i] << std::endl;
    }
    std::cout << "\n\n\nStarting sort..." << std::endl;
    quicksort(testArr, 0, 50-1, hoarePartition, medianOfThree);
    for (int i = 0; i < 50; ++i) {
        std::cout << testArr[i] << std::endl;
    }
    delete [] testArr;
    return 0;
}