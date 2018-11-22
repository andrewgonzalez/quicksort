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
void quicksort(int sortMe[], int leftIndex, int rightIndex) {
    if (leftIndex < rightIndex) {
        int splitPosition = lomutoPartition(sortMe, leftIndex, rightIndex);
        quicksort(sortMe, leftIndex, splitPosition-1);
        quicksort(sortMe, splitPosition+1, rightIndex);
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
    int i = leftIndex+1;
    int j = rightIndex;
    while (i < j) {
        while (partArr[i] < pivot) {
            ++i;
        }
        while (partArr[j] > pivot) {
            --j;
        }
        swap(partArr, i, j);
    }
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

