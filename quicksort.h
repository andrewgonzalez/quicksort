//
// Created by Andrew Gonzalez on 11/20/18.
//

#ifndef QUICKSORT_H
#define QUICKSORT_H

void quicksort(int sortMe[], int leftIndex, int rightIndex,
        int (*partitionFunc)(int*,int,int,int), int (*pivotFunc)(int*,int,int));

// Partition functions: Lomuto and Hoare partition
int lomutoPartition(int partArr[], int leftIndex, int rightIndex, int pivotIndex);
int hoarePartition(int partArr[], int leftIndex, int rightIndex, int pivotIndex);

// Pivot choosing functions
int basicPivot(int array[], int leftIndex, int rightIndex);
int randomPivot(int array[], int leftIndex, int rightIndex);
int medianOfThree(int array[], int leftIndex, int rightIndex);
int medianOfMedians(int array[], int leftIndex, int rightIndex);

// Utility functions
void swap(int arr[], int currentIndex, int swapIndex);

#endif //QUICKSORT_H
