//
// Created by Andrew Gonzalez on 11/20/18.
//

#include <iostream>
#include "quicksort.h"


// Sorts a subarray using the quicksort algorithm as the function
// name suggests.
// Input: Subarray of array sortMe[0..n-1], and the left and right indices.
// Output: an int indicating successful completion (0) or an int indicating
//    an error (any positive integer).
void quicksort(int sortMe[], int leftIndex, int rightIndex, int (*partitionFunc)(int*,int,int)) {
    if (leftIndex < rightIndex) {
        int splitPosition = partitionFunc(sortMe, leftIndex, rightIndex);
        quicksort(sortMe, leftIndex, splitPosition-1, partitionFunc);
        quicksort(sortMe, splitPosition+1, rightIndex, partitionFunc);
    }
}

int lomutoPartition(int partArr[], int leftIndex, int rightIndex) {
    int pivot = partArr[leftIndex];
    int current = leftIndex;
    for (int i = leftIndex + 1; i <= rightIndex; ++i) {
        if (partArr[i] < pivot) {
            ++current;
            swap(partArr, current, i);
        }
    }
    swap(partArr, leftIndex, current);
    return current;
}

int hoarePartition(int partArr[], int leftIndex, int rightIndex) {
    int pivot = partArr[leftIndex];
    int i = leftIndex;
    int j = rightIndex+1;
    while (i < j) {
        do {
            ++i;
        } while (partArr[i] < pivot);
        do {
            --j;
        } while (partArr[j] > pivot);
        if (i >= leftIndex && j <= rightIndex)
            swap(partArr, i, j);
    }
    swap(partArr, i, j); // undo last swap when i >= j
    swap(partArr, leftIndex, j);
    return j;
}

void swap(int arr[], int currentIndex, int swapIndex) {
    // If the indices are the same don't bother swapping.
    // Not sure if this actually saves any work since an additional comparison
    // is made.
    if (currentIndex == swapIndex)
        return;
    int temp = arr[currentIndex];
    arr[currentIndex] = arr[swapIndex];
    arr[swapIndex] = temp;
}

