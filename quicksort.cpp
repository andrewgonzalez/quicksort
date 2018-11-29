//
// Andrew Gonzalez
// CS 350
//

#include "quicksort.h"


// Sorts a subarray using the quicksort algorithm as the function
// name suggests.
// Input: Subarray of array sortMe[0..n-1], and the left and right indices.
// Output: an int indicating successful completion (0) or an int indicating
//    an error (any positive integer).
void quicksort(int sortMe[], int leftIndex, int rightIndex,
        int (*partitionFunc)(int*,int,int,int), int (*pivotFunc)(int*,int,int)) {
    if (leftIndex < rightIndex) {
        int pivotIndex = pivotFunc(sortMe, leftIndex, rightIndex);
        int splitPosition = partitionFunc(sortMe, leftIndex, rightIndex, pivotIndex);
        quicksort(sortMe, leftIndex, splitPosition-1, partitionFunc, pivotFunc);
        quicksort(sortMe, splitPosition+1, rightIndex, partitionFunc, pivotFunc);
    }
}


// The Lomuto partition.
// Input: an int array to partition, and 3 ints for the left, right, and pivot
// indices respectively.
// Output: and int of the index to split the array for sorting.
int lomutoPartition(int partArr[], int leftIndex, int rightIndex, int pivotIndex) {
    int pivot = partArr[pivotIndex];
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


// The Hoare partition.
// Input: an int array to partition, and 3 ints for the left, right, and pivot
// indices respectively.
// Output: and int of the index to split the array for sorting.
int hoarePartition(int partArr[], int leftIndex, int rightIndex, int pivotIndex) {
    int pivot = partArr[pivotIndex];
    int i = leftIndex;
    int j = rightIndex+1;
    while (i < j) {
        do {
            ++i;
        } while (partArr[i] < pivot && i < rightIndex);
        do {
            --j;
        } while (partArr[j] > pivot && j > leftIndex);
        swap(partArr, i, j);
    }
    swap(partArr, i, j); // undo last swap when i >= j
    swap(partArr, leftIndex, j);
    return j;
}


// Return the leftIndex parameter to be used as the pivot index.
// Input: an array of integers and ints for the left and right indices respectively.
// Output: an int to be used as a pivot index.
int basicPivot(int array[], int leftIndex, int rightIndex) {
    return leftIndex;
}


// Choose a random index within the subarray to be used as the pivot index.
// Input: an array of integers and ints for the left and right indices respectively.
// Output: an int to be used as a pivot index.
int randomPivot(int array[], int leftIndex, int rightIndex) {
    // Generate a random number within the indices range
    std::random_device seed;
    std::mt19937 randGen(seed());
    std::uniform_int_distribution<int> dist(leftIndex, rightIndex);
    randGen.discard(3);
    int pivotIndex = dist(randGen);
    swap(array, leftIndex, pivotIndex);
    return leftIndex;
}


// Find the median value of the left, right, and middle indices for an array.
// Input: an array of integers and ints for the left and right indices respectively.
// Output: an int to be used as a pivot index.
int medianOfThree(int array[], int leftIndex, int rightIndex) {
    int middle = (leftIndex + rightIndex) / 2;

    if (array[leftIndex] > array[middle])
        swap(array, leftIndex, middle);
    if (array[leftIndex] > array[rightIndex])
        swap(array, leftIndex, rightIndex);
    if (array[middle] > array[rightIndex])
        swap(array, middle, rightIndex);
    // Put pivot in left spot. This is for the partition functions since they
    // use the left index as the pivot.
    swap(array, leftIndex, middle);
    return leftIndex;
}


// **NOT IMPLEMENTED**
// Find the median of a list of medians.
// Input: an array of integers and ints for the left and right indices respectively.
// Output: an int to be used as a pivot index.
int medianOfMedians(int array[], int leftIndex, int rightIndex) {
    return leftIndex;
}


// Swap the values of the array at the passed in indices.
// Input: an array of integers and ints for the indices to be swapped.
// Output: none
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
